#include "playlistmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !storage)
        return 0;
    else
        return storage->playlist.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !storage)
        return QVariant();

    if (index.row() < 0 || static_cast<unsigned int>(index.row()) >= storage->playlist.size())
        return QVariant();

    if (role < Qt::UserRole || role > static_cast<int>(Role::LastRole))
        return QVariant();

    switch (static_cast<Role>(role)) {
    case Role::Name:
        return storage->playlist[index.row()].name;
    case Role::Id:
        return storage->playlist[index.row()].id;
    case Role::Url:
        return storage->playlist[index.row()].url;
    }

    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    return QHash<int, QByteArray>({
                          { static_cast<int>(Role::Name), "name" }
                        , { static_cast<int>(Role::Id),   "id" }
                        , { static_cast<int>(Role::Url),  "url" }
                                  });
}

void PlaylistModel::setChannel(const QString &newChannel)
{
    if (newChannel == mChannel)
        return;
    mChannel = newChannel;
    emit channelChanged(mChannel);

    contentType = ContentType::Stream;
    auto req = std::make_shared<Usher::StreamAccessTokenRequest>();
    currentRequest = req;
    connect(req.get(), &Request::responseReceived, this, &PlaylistModel::receiveToken);
    req->channelId = mChannel;
    Client::getClient()->send(req);
}

void PlaylistModel::setVodId(const QString &newVodId)
{
    if (newVodId == mVodId)
        return;
    mVodId = newVodId;
    emit vodIdChanged(mVodId);

    contentType = ContentType::Vod;
    auto req = std::make_shared<Usher::VodAccessTokenRequest>();
    currentRequest = req;
    connect(req.get(), &Request::responseReceived, this, &PlaylistModel::receiveToken);
    req->vodId = mVodId;
    Client::getClient()->send(req);
}

void PlaylistModel::receiveToken(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<Usher::AccessToken>(static_cast<Usher::AccessToken*>(response->object.release()));
    switch (contentType) {
    case ContentType::Stream: {
        auto req = std::make_shared<Usher::StreamPlaylistRequest>();
        currentRequest = req;
        req->channelId = mChannel;
        req->sig = data->sig;
        req->token = data->token;
        break;
    }
    case ContentType::Vod: {
        auto req = std::make_shared<Usher::VodPlaylistRequest>();
        currentRequest = req;
        req->vodId = mVodId;
        req->sig = data->sig;
        req->token = data->token;
        break;
    }
    default:
        throw std::runtime_error("Unexpected content type in PlaylistModel");
    }
    connect(currentRequest.get(), &Request::responseReceived, this, &PlaylistModel::receivePlaylist);
    Client::getClient()->send(currentRequest);
}

void PlaylistModel::receivePlaylist(const std::shared_ptr<Response> &response)
{
    if (storage && !storage->playlist.empty()) {
        beginRemoveRows(QModelIndex(), 0, storage->playlist.size() - 1);
        storage.reset();
        endRemoveRows();
    }
    storage = std::unique_ptr<Usher::Playlist>(static_cast<Usher::Playlist*>(response->object.release()));
    for (auto it = storage->playlist.begin(); it != storage->playlist.end(); ++it) {
        if (it->id == "audio_only") {
            mAudioUrl = it->url;
            emit audioUrlChanged(mAudioUrl);
            it = storage->playlist.erase(it);
            it--;
        }
    }
    beginInsertRows(QModelIndex(), 0, storage->playlist.size() - 1);
    endInsertRows();
    currentRequest.reset();
}

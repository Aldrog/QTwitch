/*
 * Copyright © 2019 Andrew Penkrat <contact.aldrog@gmail.com>
 *
 * This file is part of QTwitch.
 *
 * QTwitch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QTwitch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with QTwitch.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "followedchannelsmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

FollowedChannelsModel::FollowedChannelsModel(QObject *parent)
    : HelixScrollableModel(parent)
{
    request = std::make_shared<Helix::UserFollowsRequest>();
    request->fromId = Client::get()->getUserId();
    connect(request.get(), &Request::responseReceived, this, &FollowedChannelsModel::receiveFollows);

    usersRequest = std::make_shared<Helix::UsersRequest>();
    connect(usersRequest.get(), &Request::responseReceived, this, &FollowedChannelsModel::receiveStreams);

    streamsRequest = std::make_shared<Helix::StreamsRequest>();
    connect(streamsRequest.get(), &Request::responseReceived, this, &FollowedChannelsModel::receiveStreams);
}

QVariant FollowedChannelsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || static_cast<unsigned int>(index.row()) >= storageSize())
        return QVariant();

    if (role < Qt::UserRole || role > static_cast<int>(Role::LastRole))
        return QVariant();

    switch (static_cast<Role>(role)) {
    case Role::Image:
        return storage[index.row()].img.imageUrl;
    case Role::Title:
        return storage[index.row()].img.title;
    case Role::AdditionalData:
        return QVariant::fromValue(storage[index.row()].payload);
    }

    return QVariant();
}

void FollowedChannelsModel::receiveFollows(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<Helix::FollowsList>(static_cast<Helix::FollowsList*>(response->object.release()));
    usersRequest->id.clear();
    streamsRequest->userId.clear();
    for (const auto &follow : data->data) {
        usersRequest->id.push_back(follow.toId);
        streamsRequest->userId.push_back(follow.toId);
    }
    auto client = Client::get();
    client->send(usersRequest);
    client->send(streamsRequest);
    updateCursor(data->pagination.cursor);
}

void FollowedChannelsModel::receiveUsers(const std::shared_ptr<Response> &response)
{
    usersResponse = response;
    if (streamsResponse)
        pushData();
}

void FollowedChannelsModel::receiveStreams(const std::shared_ptr<Response> &response)
{
    streamsResponse = response;
    if (usersResponse)
        pushData();
}

void FollowedChannelsModel::pushData()
{
    auto users = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(usersResponse->object.release()));
    auto streams = std::unique_ptr<Helix::StreamsList>(static_cast<Helix::StreamsList*>(streamsResponse->object.release()));
    usersResponse.reset();
    streamsResponse.reset();
    beginInsertRows(QModelIndex(), storageSize(), storageSize() + users->data.size() - 1);
    for (const auto &user : users->data) {
        const auto &stream = std::find_if(streams->data.begin(), streams->data.end(),
                                          [&user](const Helix::StreamData &s) { return s.userId == user.id; });
        bool live = false;
        if (stream != streams->data.end())
            live = stream->type == QStringLiteral("live");

        if (live) {
            QString imgUrl = stream->thumbnailUrl;
            imgUrl.replace(QStringLiteral("{width}"), QString::number(imageWidth));
            imgUrl.replace(QStringLiteral("{height}"), QString::number(imageHeight()));
            storage.emplace_back( EntitledImage(imgUrl, user.displayName),
                                  FollowedChannelPayload(live, stream->title, stream->viewerCount) );
        } else {
            storage.emplace_back( EntitledImage(user.offlineImageUrl, user.displayName),
                                  FollowedChannelPayload(live, QString(), 0) );
        }
    }
    endInsertRows();
}

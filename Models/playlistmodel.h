#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <Api/Usher/endpoints.h>
#include <Api/Helix/endpoints.h>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(QString vodId READ vodId WRITE setVodId NOTIFY vodIdChanged)
    Q_PROPERTY(QString audioUrl READ audioUrl NOTIFY audioUrlChanged)
public:
    enum class Role : int {
        Name = Qt::UserRole,
        Id,
        Url,
        LastRole = Url
    };

    explicit PlaylistModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const final;
    QVariant data(const QModelIndex &index, int role) const final;
    QHash<int, QByteArray> roleNames() const final;

    inline QString channel() const { return mChannel; }
    inline QString vodId() const { return mVodId; }
    inline QString audioUrl() const { return mAudioUrl; }

    void setChannel(const QString &newChannel);
    void setVodId(const QString &newVodId);

signals:
    void channelChanged(const QString &newChannel);
    void vodIdChanged(const QString &newVodId);
    void audioUrlChanged(const QString &newAurioUrl);

private:
    std::unique_ptr<Api::Usher::Playlist> storage;

    enum class ContentType {
        Stream, Vod, Undefined
    } contentType = ContentType::Undefined;

    std::shared_ptr<Api::Request> currentRequest;

    QString mChannel;
    QString mVodId;
    QString mAudioUrl;

private slots:
    void receiveToken(const std::shared_ptr<QTwitch::Api::Response> &response);
    void receivePlaylist(const std::shared_ptr<QTwitch::Api::Response> &response);
};

}
}

#endif // PLAYLISTMODEL_H

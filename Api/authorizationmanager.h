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

#ifndef AUTHORIZATIONMANAGER_H
#define AUTHORIZATIONMANAGER_H

#include "qtwitch_global.h"
#include "credentials.h"
#include "request.h"
#include <QObject>
#include <QUrl>

namespace QTwitch {
namespace Api {

class QTWITCHSHARED_EXPORT AuthorizationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
public:
    explicit AuthorizationManager(QObject *parent = nullptr);

    enum class Status
    {
        Authorized,
        NotAuthorized,
        Invalid
    };
    Q_ENUM(Status)

    enum class Error
    {
        VerificationFailed = 1,
        InvalidRedirectUrl,
        StateMismatch
    };
    Q_ENUM(Error)

    enum class Scope
    {
        // New API
        AnalyticsReadExtensions,
        AnalyticsReadGames,
        BitsRead,
        ClipsEdit,
        UserEdit,
        UserEditBroadcast,
        UserReadBroadcast,
        UserReadEmail,

        // API v5
        ChannelCheckSubscription,
        ChannelCommercial,
        ChannelEditor,
        ChannelFeedEdit,
        ChannelFeedRead,
        ChannelRead,
        ChannelStream,
        ChannelSubscriptions,
        CollectionsEdit,
        CommunitiesEdit,
        OpenId,
        UserBlocksEdit,
        UserBlocksRead,
        UserFollowsEdit,
        UserRead,
        UserSubscriptions,
        ViewingActivityRead,

        // Chat
        ChannelModerate,
        ChatEdit,
        ChatRead,
        WhispersRead,
        WhispersEdit
    };
    Q_ENUM(Scope)

    QNetworkRequest authorize(const std::shared_ptr<Request> &request) const;
    QByteArray authorizePost(const std::shared_ptr<Request> &request) const;

    inline QString userId() const { return credentials.userId; }
    inline Status status() const { return mStatus; }
    void setCredentialsStorage(std::unique_ptr<AbstractCredentialsStorage> storage);
    Q_INVOKABLE QUrl init(const std::vector<Scope> &scopes, bool force = false);

signals:
    void completed();
    void statusChanged(Status status);
    void error(Error e);

public slots:
    void update(const QUrl &url);
    void erase();
    void verify();

private:
    std::unique_ptr<AbstractCredentialsStorage> credentialsStorage;

    Credentials credentials;

    void setStatus(Status status);

    QString authorizationStateParameter;

    static QString generateRandomString(int from, int to);
    static QString scopeToString(Scope scope);

    Status mStatus = Status::NotAuthorized;
};

}
}

#endif // AUTHORIZATIONMANAGER_H

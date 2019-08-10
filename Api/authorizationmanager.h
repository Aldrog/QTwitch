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

    enum class Status {
        Authorized,
        InProgress,
        NotAuthorized,
        Invalid
    };
    Q_ENUM(Status)

    enum class Error {
        VerificationFailed = 1,
        InvalidRedirectUrl,
        StateMismatch
    };
    Q_ENUM(Error)

    enum class Scope {
        // New API
        AnalyticsReadExtensions,  /* View analytics data for your extensions. */
        AnalyticsReadGames,       /* View analytics data for your games. */
        BitsRead,                 /* View Bits information for your channel. */
        ChannelReadSubscriptions, /* Get a list of all subscribers to your channel
                                     and check if a user is subscribed to your
                                     channel. */
        ClipsEdit,                /* Manage a clip object. */
        UserEdit,                 /* Manage a user object. */
        UserEditBroadcast,        /* Edit your channel’s broadcast configuration,
                                     including extension configuration. (This scope
                                     implies UserReadBroadcast capability.) */
        UserReadBroadcast,        /* View your broadcasting configuration, including
                                     extension configurations. */
        UserReadEmail,            /* Read authorized user’s email address. */

        // API v5
        ChannelCheckSubscription, /* Read whether a user is subscribed to your
                                     channel. */
        ChannelCommercial,        /* Trigger commercials on channel. */
        ChannelEditor,            /* Write channel metadata (game, status, etc). */
        ChannelFeedEdit,          /* Add posts and reactions to a channel feed. */
        ChannelFeedRead,          /* View a channel feed. */
        ChannelRead,              /* Read nonpublic channel information, including email
                                     address and stream key. */
        ChannelStream,            /* Reset a channel’s stream key. */
        ChannelSubscriptions,     /* Read all subscribers to your channel. */
        CollectionsEdit,          /* Manage a user’s collections (of videos). */
        CommunitiesEdit,          /* Manage a user’s communities. */
        CommunitiesModerate,      /* Manage community moderators. */
        OpenId,                   /* Use OpenID Connect authentication. */
        UserBlocksEdit,           /* Turn on/off ignoring a user. Ignoring users means you
                                     cannot see them type, receive messages from them, etc. */
        UserBlocksRead,           /* Read a user’s list of ignored users. */
        UserFollowsEdit,          /* Manage a user’s followed channels. */
        UserRead,                 /* Read nonpublic user information, like email address. */
        UserSubscriptions,        /* Read a user’s subscriptions. */
        ViewingActivityRead,      /* Turn on Viewer Heartbeat Service ability to record
                                     user data. */

        // Chat
        ChannelModerate, /* Perform moderation actions in a channel. The user
                            requesting the scope must be a moderator in the
                            channel. */
        ChatEdit,        /* Send live stream chat and rooms messages. */
        ChatRead,        /* View live stream chat and rooms messages. */
        WhispersRead,    /* View your whisper messages. */
        WhispersEdit,    /* Send whisper messages. */

        First = AnalyticsReadExtensions,
        Last = WhispersEdit
    };
    Q_ENUM(Scope)

    QNetworkRequest authorize(const std::shared_ptr<Request> &request) const;
    QByteArray authorizePost(const std::shared_ptr<Request> &request) const;

    inline QString userId() const { return credentials.userId; }
    inline QString login() const { return credentials.login; }
    inline Status status() const { return mStatus; }
    void setCredentialsStorage(std::unique_ptr<AbstractCredentialsStorage> storage);
    Q_INVOKABLE QUrl init(const QList<Scope> &scopes, bool force = false);
    Q_INVOKABLE QUrl init(const QList<int> &scopes, bool force = false);

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

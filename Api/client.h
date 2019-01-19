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

#ifndef CLIENT_H
#define CLIENT_H

#include "request.h"
#include "response.h"
#include "credentials.h"

#include "qtwitch_global.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace QTwitch {
namespace Api {

class QTWITCHSHARED_EXPORT Client : public QObject
{
Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void setCredentialsStorage(std::unique_ptr<AbstractCredentialsStorage> storage);

    enum class AuthorizationError
    {
        VerificationFailed = 1,
        InvalidRedirectUrl,
        StateMismatch
    };
    Q_ENUM(AuthorizationError)

    enum class AuthorizationScope
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
    Q_ENUM(AuthorizationScope)

public slots:
    void send(const std::shared_ptr<QTwitch::Api::Request> &request);

    QUrl initiateAuthorization(const std::vector<AuthorizationScope> &scopes, bool force = false);
    void updateAuthorization(const QUrl &url);
    void eraseAuthorization();
    bool verifyAuthorization();

signals:
    void receive(const std::shared_ptr<QTwitch::Api::Response> &response);

    void error(QNetworkReply::NetworkError e);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &e);

    void authorizationCompleted();
    void browserRequired(const QUrl &url);
    void authorizationError(AuthorizationError e);

private:
    std::unique_ptr<QNetworkAccessManager> network;
    std::unique_ptr<AbstractCredentialsStorage> credentialsStorage;

    Credentials credentials;

    QString authorizationStateParameter;

    static QString generateRandomString(int from, int to);
    static QString scopeToString(AuthorizationScope scope);
};

}
}

#endif // CLIENT_H

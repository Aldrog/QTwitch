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

#include "client.h"
#include <QtDebug>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QRandomGenerator>

using namespace QTwitch::Api;

Client::Client(QObject *parent)
    : QObject(parent)
    , network(std::make_unique<QNetworkAccessManager>())
{
    connect(network.get(), &QNetworkAccessManager::sslErrors, this, &Client::sslErrors);
}

void Client::setCredentialsStorage(std::unique_ptr<AbstractCredentialsStorage> storage)
{
    credentialsStorage = std::move(storage);
    credentialsStorage->readCredentials(credentials);
}

void Client::send(const std::shared_ptr<Request> &request)
{
    QNetworkRequest qreq(request->getFullUrl());
    qreq.setRawHeader("Client-ID", TWITCH_CLIENT_ID);
    QNetworkReply *qrep = network->get(qreq);
    connect(qrep, &QNetworkReply::readyRead, [&] ()
    {
        auto responseObject = request->createResponseObject(qrep->readAll());
        auto response = std::make_shared<Response>();
        response->object = std::move(responseObject);
        response->request = request;
        emit receive(response);
        emit request->responseReceived(response);
        qrep->deleteLater();
    });
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Client::error);
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), request.get(), &Request::error);
}

QUrl Client::initiateAuthorization(const std::vector<AuthorizationScope> &scopes, bool force)
{
    assert(credentialsStorage);
    assert(!scopes.empty());

    QUrl url(QStringLiteral("https://id.twitch.tv/oauth2/authorize"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("client_id"),
                       QStringLiteral(TWITCH_CLIENT_ID));
    query.addQueryItem(QStringLiteral("redirect_uri"),
                       QStringLiteral(TWITCH_REDIRECT_URI));
    query.addQueryItem(QStringLiteral("response_type"),
                       QStringLiteral("token"));
    if (force)
        query.addQueryItem(QStringLiteral("force_verify"),
                           QStringLiteral("true"));

    QString scopesParam;
    for (const auto &s : scopes) {
        if(scopesParam.isNull())
            scopesParam = scopeToString(s);
        else
            scopesParam += QStringLiteral("+") + scopeToString(s);
    }
    query.addQueryItem(QStringLiteral("scope"),
                       scopesParam);

    authorizationStateParameter = generateRandomString(43, 128);
    query.addQueryItem(QStringLiteral("state"),
                       authorizationStateParameter);
    url.setQuery(query);
    return url;
}

void Client::updateAuthorization(const QUrl &url)
{
    assert(credentialsStorage);

    if (!url.hasFragment()) {
        emit authorizationError(AuthorizationError::InvalidRedirectUrl);
        return;
    }

    QString fragment = url.fragment();

    QUrlQuery query(url);
    // QUrl fails parsing if fragment comes before query
    if (query.isEmpty()) {
        query = QUrlQuery(fragment);
        fragment = fragment.left(fragment.indexOf('&'));
    }

    if (!query.hasQueryItem(QStringLiteral("state"))) {
        emit authorizationError(AuthorizationError::InvalidRedirectUrl);
        return;
    }
    if (query.queryItemValue(QStringLiteral("state")) != authorizationStateParameter) {
        emit authorizationError(AuthorizationError::StateMismatch);
        return;
    }

    static const QString key = QStringLiteral("access_token=");
    static constexpr std::size_t keyLength = 13;

    if (!fragment.startsWith(key)) {
        emit authorizationError(AuthorizationError::InvalidRedirectUrl);
        return;
    }

    fragment.remove(0, keyLength);
    credentials.authToken = std::move(fragment);

    if (!verifyAuthorization()) {
        emit authorizationError(AuthorizationError::VerificationFailed);
        return;
    }

    credentialsStorage->writeCredentials(credentials);
    emit authorizationCompleted();
}

void Client::eraseAuthorization()
{
    assert(credentialsStorage);
    credentials.clear();
    credentialsStorage->clearCredentials();
}

bool Client::verifyAuthorization()
{
    return true;
}

QString Client::generateRandomString(int from, int to)
{
    auto rng = QRandomGenerator::global();
    int size = rng->bounded(from, to);
    QString result;
    for (int i = 0; i < size; ++i) {
        char symbol = rng->bounded(66);
        if (symbol < 26)
            symbol += 'A';
        else if ((symbol -= 26) < 26)
            symbol += 'a';
        else if ((symbol -= 26) < 10)
            symbol += '0';
        else if ((symbol -= 10) == 0)
            symbol = '-';
        else if ((--symbol) == 0)
            symbol = '.';
        else if ((--symbol) == 0)
            symbol = '_';
        else if ((--symbol) == 0)
            symbol = '~';
        result.push_back(symbol);
    }
    return result;
}

QString Client::scopeToString(AuthorizationScope scope)
{
    switch (scope) {
    // New API
    case AuthorizationScope::AnalyticsReadExtensions:
        return QStringLiteral("analytics:read:extensions");
    case AuthorizationScope::AnalyticsReadGames:
        return QStringLiteral("analytics:read:games");
    case AuthorizationScope::BitsRead:
        return QStringLiteral("bits:read");
    case AuthorizationScope::ClipsEdit:
        return QStringLiteral("clips:edit");
    case AuthorizationScope::UserEdit:
        return QStringLiteral("user:edit");
    case AuthorizationScope::UserEditBroadcast:
        return QStringLiteral("user:edit:broadcast");
    case AuthorizationScope::UserReadBroadcast:
        return QStringLiteral("user:read:broadcast");
    case AuthorizationScope::UserReadEmail:
        return QStringLiteral("user:read:email");

    // API v5
    case AuthorizationScope::ChannelCheckSubscription:
        return QStringLiteral("channel_check_subscription");
    case AuthorizationScope::ChannelCommercial:
        return QStringLiteral("channel_commercial");
    case AuthorizationScope::ChannelEditor:
        return QStringLiteral("channel_editor");
    case AuthorizationScope::ChannelFeedEdit:
        return QStringLiteral("channel_feed_edit");
    case AuthorizationScope::ChannelFeedRead:
        return QStringLiteral("channel_feed_read");
    case AuthorizationScope::ChannelRead:
        return QStringLiteral("channel_read");
    case AuthorizationScope::ChannelStream:
        return QStringLiteral("channel_stream");
    case AuthorizationScope::ChannelSubscriptions:
        return QStringLiteral("channel_subscriptions");
    case AuthorizationScope::CollectionsEdit:
        return QStringLiteral("collections_edit");
    case AuthorizationScope::CommunitiesEdit:
        return QStringLiteral("communities_edit");
    case AuthorizationScope::OpenId:
        return QStringLiteral("openid");
    case AuthorizationScope::UserBlocksEdit:
        return QStringLiteral("user_blocks_edit");
    case AuthorizationScope::UserBlocksRead:
        return QStringLiteral("user_blocks_read");
    case AuthorizationScope::UserFollowsEdit:
        return QStringLiteral("user_follows_edit");
    case AuthorizationScope::UserRead:
        return QStringLiteral("user_read");
    case AuthorizationScope::UserSubscriptions:
        return QStringLiteral("user_subscriptions");
    case AuthorizationScope::ViewingActivityRead:
        return QStringLiteral("viewing_activity_read");

    // Chat
    case AuthorizationScope::ChannelModerate:
        return QStringLiteral("channel:moderate");
    case AuthorizationScope::ChatEdit:
        return QStringLiteral("chat:edit");
    case AuthorizationScope::ChatRead:
        return QStringLiteral("chat:read");
    case AuthorizationScope::WhispersRead:
        return QStringLiteral("whispers:read");
    case AuthorizationScope::WhispersEdit:
        return QStringLiteral("whispers:edit");
    }

    // To suppress compiler (false?) warning
    return QString();
}

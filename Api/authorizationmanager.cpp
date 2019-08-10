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

#include "authorizationmanager.h"
#include "authorizationflow.h"
#include "client.h"
#include <QUrlQuery>
#include <QRandomGenerator>

using namespace QTwitch::Api;

AuthorizationManager::AuthorizationManager(QObject *parent)
    : QObject(parent)
{
}

QNetworkRequest AuthorizationManager::authorize(const std::shared_ptr<Request> &request) const
{
    if (mStatus == Status::Authorized || mStatus == Status::InProgress)
        return request->getNetworkRequest(credentials.authToken);
    else
        return request->getNetworkRequest({});
}

QByteArray AuthorizationManager::authorizePost(const std::shared_ptr<Request> &request) const
{
    if (mStatus == Status::Authorized || mStatus == Status::InProgress)
        return request->postData(credentials.authToken);
    else
        return request->postData({});
}

void AuthorizationManager::setCredentialsStorage(std::unique_ptr<AbstractCredentialsStorage> storage)
{
    credentialsStorage = std::move(storage);
    credentialsStorage->readCredentials(credentials);
    if (!credentials.authToken.isEmpty()) {
        setStatus(Status::Authorized);
        verify();
    }
}

QUrl AuthorizationManager::init(const QList<Scope> &scopes, bool force)
{
    assert(credentialsStorage);

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
    qInfo() << "Scopes:";
    for (const auto &s : scopes) {
        qInfo() << s;
        if (scopesParam.isNull())
            scopesParam = scopeToString(s);
        else
            scopesParam += QStringLiteral("+") + scopeToString(s);
    }
    if (!scopesParam.isNull()) {
        query.addQueryItem(QStringLiteral("scope"),
                           scopesParam);
    }

    authorizationStateParameter = generateRandomString(43, 128);
    query.addQueryItem(QStringLiteral("state"),
                       authorizationStateParameter);
    url.setQuery(query);
    return url;
}

QUrl AuthorizationManager::init(const QList<int> &scopes, bool force) {
    QList<Scope> validScopes;
    for (const auto &s : scopes) {
        if (s < static_cast<int>(Scope::First) || s > static_cast<int>(Scope::Last)) {
            qCritical() << "Invalid login scope " << s;
            return {};
        }
        validScopes.push_back(static_cast<Scope>(s));
    }
    return init(validScopes, force);
}

void AuthorizationManager::update(const QUrl &url)
{
    assert(credentialsStorage);

    if (!url.hasFragment()) {
        emit error(Error::InvalidRedirectUrl);
        return;
    }

    QString fragment = url.fragment();

    QUrlQuery query(url);
    // QUrl fails parsing if fragment comes before query
    if (query.isEmpty()) {
        query.setQuery(fragment);
        fragment = fragment.left(fragment.indexOf('&'));
    }

    if (!query.hasQueryItem(QStringLiteral("state"))) {
        emit error(Error::InvalidRedirectUrl);
        return;
    }
    if (query.queryItemValue(QStringLiteral("state")) != authorizationStateParameter) {
        emit error(Error::StateMismatch);
        return;
    }

    static const QString key = QStringLiteral("access_token=");
    static constexpr std::size_t keyLength = 13;

    if (!fragment.startsWith(key)) {
        emit error(Error::InvalidRedirectUrl);
        return;
    }

    fragment.remove(0, keyLength);
    credentials.authToken = std::move(fragment);
    setStatus(Status::InProgress);
    verify();
}

void AuthorizationManager::erase()
{
    assert(credentialsStorage);

    using namespace Authorization;
    auto request = std::make_shared<RevokeRequest>();
    Client::get()->send(request);

    credentials.clear();
    credentialsStorage->clearCredentials();

    setStatus(Status::NotAuthorized);
}

void AuthorizationManager::verify()
{
    using namespace Authorization;
    auto request = std::make_shared<ValidateRequest>();
    connect(request.get(), &Request::responseReceived, [this] (const std::shared_ptr<Response> &responce)
    {
        if (responce->status != QNetworkReply::NoError) {
            setStatus(Status::Invalid);
            return;
        }
        auto data = std::static_pointer_cast<ValidationData>(std::shared_ptr<Object>(move(responce->object)));
        credentials.userId = data->userId;
        credentialsStorage->writeCredentials(credentials);
        setStatus(Status::Authorized);
        emit completed();
    });
    Client::get()->send(request);
}

void AuthorizationManager::setStatus(Status status)
{
    if (mStatus != status) {
        mStatus = status;
        emit statusChanged(mStatus);
    }
}

QString AuthorizationManager::generateRandomString(int from, int to)
{
    auto rng = QRandomGenerator::global();
    int size = rng->bounded(from, to);
    QString result;
    for (int i = 0; i < size; ++i) {
        auto symbol = static_cast<char>(rng->bounded(66));
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

QString AuthorizationManager::scopeToString(Scope scope)
{
    switch (scope) {
    // New API
    case Scope::AnalyticsReadExtensions:
        return QStringLiteral("analytics:read:extensions");
    case Scope::AnalyticsReadGames:
        return QStringLiteral("analytics:read:games");
    case Scope::BitsRead:
        return QStringLiteral("bits:read");
    case Scope::ChannelReadSubscriptions:
        return QStringLiteral("channel:read:subscriptions");
    case Scope::ClipsEdit:
        return QStringLiteral("clips:edit");
    case Scope::UserEdit:
        return QStringLiteral("user:edit");
    case Scope::UserEditBroadcast:
        return QStringLiteral("user:edit:broadcast");
    case Scope::UserReadBroadcast:
        return QStringLiteral("user:read:broadcast");
    case Scope::UserReadEmail:
        return QStringLiteral("user:read:email");

    // API v5
    case Scope::ChannelCheckSubscription:
        return QStringLiteral("channel_check_subscription");
    case Scope::ChannelCommercial:
        return QStringLiteral("channel_commercial");
    case Scope::ChannelEditor:
        return QStringLiteral("channel_editor");
    case Scope::ChannelFeedEdit:
        return QStringLiteral("channel_feed_edit");
    case Scope::ChannelFeedRead:
        return QStringLiteral("channel_feed_read");
    case Scope::ChannelRead:
        return QStringLiteral("channel_read");
    case Scope::ChannelStream:
        return QStringLiteral("channel_stream");
    case Scope::ChannelSubscriptions:
        return QStringLiteral("channel_subscriptions");
    case Scope::CollectionsEdit:
        return QStringLiteral("collections_edit");
    case Scope::CommunitiesEdit:
        return QStringLiteral("communities_edit");
    case Scope::CommunitiesModerate:
        return QStringLiteral("communities_moderate");
    case Scope::OpenId:
        return QStringLiteral("openid");
    case Scope::UserBlocksEdit:
        return QStringLiteral("user_blocks_edit");
    case Scope::UserBlocksRead:
        return QStringLiteral("user_blocks_read");
    case Scope::UserFollowsEdit:
        return QStringLiteral("user_follows_edit");
    case Scope::UserRead:
        return QStringLiteral("user_read");
    case Scope::UserSubscriptions:
        return QStringLiteral("user_subscriptions");
    case Scope::ViewingActivityRead:
        return QStringLiteral("viewing_activity_read");

    // Chat
    case Scope::ChannelModerate:
        return QStringLiteral("channel:moderate");
    case Scope::ChatEdit:
        return QStringLiteral("chat:edit");
    case Scope::ChatRead:
        return QStringLiteral("chat:read");
    case Scope::WhispersRead:
        return QStringLiteral("whispers:read");
    case Scope::WhispersEdit:
        return QStringLiteral("whispers:edit");
    }

    // To suppress compiler false warning
    return QString();
}

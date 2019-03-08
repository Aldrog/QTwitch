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

#ifndef V5_ENDPOINTS_H
#define V5_ENDPOINTS_H

#include <Api/request.h>
#include "data.h"

namespace QTwitch {
namespace Api {
namespace v5 {

class Request : public QTwitch::Api::Request
{
public:
    QString baseUrl() const final { return QStringLiteral("https://api.twitch.tv/kraken/"); }
    QNetworkRequest getNetworkRequest(const std::optional<QString> &authorization) const final;
};

class LegacyPagedRequest : public Request
{
    typedef Request Base;
public:
    std::optional<int> limit;
    std::optional<int> offset;

protected:
    QUrlQuery getQuery() const override;
};

class QTWITCHSHARED_EXPORT FollowChannelRequest final : public Request
{
    typedef Request Base;
    typedef FollowData ResponseObjectType;
public:
    std::optional<bool> notifications;

    QString userId;
    QString channelId;

    QString endpoint() const final { return QStringLiteral("users/%1/follows/channels/%2").arg(userId).arg(channelId); }
    RequestType requestType() const final { return RequestType::Put; }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

class QTWITCHSHARED_EXPORT UnfollowChannelRequest final : public Request
{
    typedef Request Base;
    typedef Object ResponseObjectType;
    // This endpoint doesn't return any data
    // Only status code
public:
    QString userId;
    QString channelId;

    QString endpoint() const final { return QStringLiteral("users/%1/follows/channels/%2").arg(userId).arg(channelId); }
    RequestType requestType() const final { return RequestType::Delete; }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;
};

class QTWITCHSHARED_EXPORT SearchChannelsRequest final : public LegacyPagedRequest
{
    typedef LegacyPagedRequest Base;
    typedef ChannelsList ResponseObjectType;
public:
    QString searchQuery;

    QString endpoint() const final { return QStringLiteral("search/channels"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

class QTWITCHSHARED_EXPORT SearchGamesRequest final : public Request
{
    typedef Request Base;
    typedef GamesList ResponseObjectType;
public:
    QString searchQuery;
    std::optional<bool> hls;

    QString endpoint() const final { return QStringLiteral("search/games"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;

private:
    QString type = QStringLiteral("suggest");
};

class QTWITCHSHARED_EXPORT SearchStreamsRequest final : public LegacyPagedRequest
{
    typedef LegacyPagedRequest Base;
    typedef StreamsList ResponseObjectType;
public:
    QString searchQuery;
    std::optional<bool> live;

    QString endpoint() const final { return QStringLiteral("search/streams"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

}
}
}

#endif // V5_ENDPOINTS_H

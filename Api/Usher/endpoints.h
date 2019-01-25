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

#ifndef USHER_ENDPOINTS_H
#define USHER_ENDPOINTS_H

#include <Api/request.h>
#include "data.h"

namespace QTwitch {
namespace Api {
namespace Usher {

class AccessTokenRequest : public Request
{
    typedef Request Base;
    typedef AccessToken ResponseObjectType;
public:
    QString baseUrl() const final { return QStringLiteral("https://api.twitch.tv/api/"); }
    std::optional<QString> authorizationPrefix() const final { return {}; }
    void setAuthorization(const QString &token) final { oauthToken = token; }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;

private:
    std::optional<QString> oauthToken;
    bool needHttps = true;
    QString platform = "web";
    QString playerBackend = "mediaplayer";
    QString playerType = "site";
};

class QTWITCHSHARED_EXPORT StreamAccessTokenRequest final : public AccessTokenRequest
{
    typedef AccessTokenRequest Base;
    typedef AccessToken ResponseObjectType;
public:
    QString channelId;

    QString endpoint() const final { return QStringLiteral("channels/%1/access_token").arg(channelId); }
};

class QTWITCHSHARED_EXPORT VodAccessTokenRequest final : public AccessTokenRequest
{
    typedef AccessTokenRequest Base;
    typedef AccessToken ResponseObjectType;
public:
    QString vodId;

    QString endpoint() const final { return QStringLiteral("vods/%1/access_token").arg(vodId); }
};

class PlaylistRequest : public Request
{
    typedef Request Base;
    typedef Playlist ResponseObjectType;
public:
    std::optional<bool> allowAudioOnly = true;

    QString sig;
    QString token;

    QString baseUrl() const final { return QStringLiteral("https://usher.ttvnw.net/"); }
    std::optional<QString> authorizationPrefix() const final { return {}; }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const override;

private:
    bool allowSource = true;
    bool bakingBread = true;
    bool bakingBrownies = true;
    int bakingBrowniesTimeout = 1050;
    bool fastBread = true;
    QString playerBackend = "mediaplayer";
    bool playlistIncludeFramerate = true;
    bool reassignmentsSupported = true;
    QString rtqos = "business_logic_reverse";
    QString cdm = "wv";
};

class QTWITCHSHARED_EXPORT StreamPlaylistRequest final : public PlaylistRequest
{
    typedef PlaylistRequest Base;
    typedef Playlist ResponseObjectType;
public:
    QString channelId;

    QString endpoint() const final { return QStringLiteral("api/channel/hls/%1.m3u8").arg(channelId); }

protected:
    QUrlQuery getQuery() const final;
};

class QTWITCHSHARED_EXPORT VodPlaylistRequest final : public PlaylistRequest
{
    typedef PlaylistRequest Base;
    typedef Playlist ResponseObjectType;
public:
    QString vodId;

    QString endpoint() const final { return QStringLiteral("vod/%1.m3u8").arg(vodId); }

protected:
    QUrlQuery getQuery() const final;
};

}
}
}

#endif // USHER_ENDPOINTS_H

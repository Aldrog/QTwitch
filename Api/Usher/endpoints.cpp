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

#include "endpoints.h"
#include "jsonsetter.h"
#include <QUrlQuery>
#include <QRandomGenerator>
#include <regex>

using namespace QTwitch::Api;
using namespace QTwitch::Api::Usher;

QUrlQuery AccessTokenRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("oauth_token"), oauthToken);
    addParam(query, QStringLiteral("need_https"), needHttps);
    addParam(query, QStringLiteral("platform"), platform);
    addParam(query, QStringLiteral("player_backend"), playerBackend);
    addParam(query, QStringLiteral("player_type"), playerType);
    return query;
}

QUrlQuery PlaylistRequest::getQuery() const
{
    int p = QRandomGenerator::global()->bounded(1e8 - 1);
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("allow_source"), allowSource);
    addParam(query, QStringLiteral("allow_audio_only"), allowAudioOnly);
    addParam(query, QStringLiteral("baking_bread"), bakingBread);
    addParam(query, QStringLiteral("baking_brownies"), bakingBrownies);
    addParam(query, QStringLiteral("baking_brownies_timeout"), bakingBrowniesTimeout);
    addParam(query, QStringLiteral("p"), p);
    addParam(query, QStringLiteral("fast_bread"), fastBread);
    addParam(query, QStringLiteral("player_backend"), playerBackend);
    addParam(query, QStringLiteral("playlist_include_framerate"), playlistIncludeFramerate);
    addParam(query, QStringLiteral("reassignments_supported"), reassignmentsSupported);
    addParam(query, QStringLiteral("rtqos"), rtqos);
    addParam(query, QStringLiteral("cdm"), cdm);
    return query;
}

QUrlQuery StreamPlaylistRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("sig"), sig);
    addParam(query, QStringLiteral("token"), QString(QUrl::toPercentEncoding(token)));
    return query;
}

QUrlQuery VodPlaylistRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("nauthsig"), sig);
    addParam(query, QStringLiteral("nauth"), QString(QUrl::toPercentEncoding(token)));
    return query;
}

GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(AccessTokenRequest)

bool startsWith(const std::string_view &v, const std::string_view &x)
{
    return v.size() >= x.size() && v.compare(0, x.size(), x) == 0;
}

std::pair<std::string_view, std::string_view> extractKeyValue(std::string_view &line)
{
    line.remove_prefix(1);
    std::string_view key = line.substr(0, line.find('='));
    line.remove_prefix(key.size() + 1);
    std::string_view value = line.substr(0, line.find(','));
    line.remove_prefix(value.size());

    return {key, value};
}

std::unique_ptr<Object> PlaylistRequest::createResponseObject(const QByteArray &data) const
{
    static constexpr std::string_view mediaPrefix("#EXT-X-MEDIA:");
    static constexpr std::string_view infoPrefix("#EXT-X-STREAM-INF:");

    static constexpr std::string_view idKey("GROUP-ID");
    static constexpr std::string_view nameKey("NAME");

    auto result = std::make_unique<ResponseObjectType>();
    PlaylistItem *pi = nullptr;
    auto lineStart = data.begin() - 1;
    while (lineStart != data.end()) {
        ++lineStart;
        auto lineEnd = std::find(lineStart, data.end(), '\n');
        std::string_view line(lineStart, lineEnd-lineStart);
        if (startsWith(line, mediaPrefix)) {
            // C++11 compatibility
            // pi = &result->playlist.emplace_back();
            result->playlist.emplace_back();
            pi = &result->playlist.back();

            line.remove_prefix(mediaPrefix.size() - 1);
            while (!line.empty()) {
                // C++11 compatibility
                // auto [key, value]
                std::string_view key, value;
                std::tie (key, value) = extractKeyValue(line);

                if (key == idKey)
                    pi->id = QString::fromUtf8(value.data()+1, value.size()-2);
                if (key == nameKey)
                    pi->name = QString::fromUtf8(value.data()+1, value.size()-2);
            }
        }
        else if (startsWith(line, infoPrefix)) {
            // Nothing of interest here for now
        }
        else if (pi && pi->url.isEmpty()) {
            pi->url = QString::fromUtf8(line.data(), line.size());
        }

        lineStart = lineEnd;
    }
    return std::move(result);
}

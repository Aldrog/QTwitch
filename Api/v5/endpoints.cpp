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

using namespace QTwitch::Api;
using namespace QTwitch::Api::v5;

QUrlQuery LegacyPagedRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("limit"), limit);
    addParam(query, QStringLiteral("offset"), offset);
    return query;
}

QUrlQuery FollowChannelRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("notifications"), notifications);
    return query;
}

QUrlQuery SearchChannelsRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("query"), searchQuery);
    return query;
}

QUrlQuery SearchGamesRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("query"), searchQuery);
    addParam(query, QStringLiteral("type"), type);
    return query;
}

QUrlQuery SearchStreamsRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("query"), searchQuery);
    return query;
}

std::unique_ptr<Object> UnfollowChannelRequest::createResponseObject(const QByteArray &data) const
{
    Q_UNUSED(data);
    return std::make_unique<Object>();
}

QNetworkRequest v5::Request::getNetworkRequest(const std::optional<QString> &authorization) const
{
    auto result = QTwitch::Api::Request::getNetworkRequest(authorization);
    if (authorization)
        result.setRawHeader("Authorization", (QStringLiteral("OAuth ") + *authorization).toUtf8());
    result.setRawHeader("Accept", "application/vnd.twitchtv.v5+json");
    return result;
}

GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(FollowChannelRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(SearchChannelsRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(SearchGamesRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(SearchStreamsRequest)

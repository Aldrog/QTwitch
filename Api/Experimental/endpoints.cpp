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
using namespace QTwitch::Api::Experimental;

QNetworkRequest Experimental::Request::getNetworkRequest(const std::optional<QString> &authorization) const
{
    QUrl url(getUrlString());
    auto query = getQuery();
    addParam(query, QStringLiteral("oauth_token"), authorization);
    url.setQuery(getQuery());
    QNetworkRequest result(url);
    result.setRawHeader("Client-ID", TWITCH_CLIENT_ID);
    return result;
}

GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(FollowGameRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(GetFollowGameRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(FollowedGamesRequest)

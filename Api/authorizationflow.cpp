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

#include "authorizationflow.h"
#include "jsonsetter.h"
#include <QUrlQuery>

using namespace QTwitch::Api::Authorization;
using namespace QTwitch::Api;

QNetworkRequest ValidateRequest::getNetworkRequest(const std::optional<QString> &authorization) const
{
    auto result = QTwitch::Api::Request::getNetworkRequest(authorization);
    if (authorization)
        result.setRawHeader("Authorization", (QStringLiteral("OAuth ") + *authorization).toUtf8());
    return result;
}

QNetworkRequest RevokeRequest::getNetworkRequest(const std::optional<QString> &authorization) const
{
    Q_UNUSED(authorization)
    QUrl url(getUrlString());
    auto query = getQuery();
    url.setQuery(getQuery());
    QNetworkRequest result(url);
    result.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    return result;
}

QByteArray RevokeRequest::postData(const std::optional<QString> &authorization) const
{
    QUrlQuery query;
    addParam(query, QStringLiteral("client_id"), QStringLiteral(TWITCH_CLIENT_ID));
    addParam(query, QStringLiteral("token"), authorization);
    return query.toString().toUtf8();
}

GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(ValidateRequest)

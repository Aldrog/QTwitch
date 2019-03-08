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
using namespace QTwitch::Api::Helix;

QUrlQuery PagedRequest::getQuery() const
{
    if (after && before)
        qWarning() << "Both after and before paging parameters are set.\n"
                      "Please check your usage of the API.";
    if (first && (*first <= 0 || *first > 100))
        qWarning() << "Paging parameter \"first\" should be more than zero and not more than 100.\n"
                      "Please check your usage of the API.";

    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("after"), after);
    addParam(query, QStringLiteral("before"), before);
    addParam(query, QStringLiteral("first"), first);
    return query;
}

QUrlQuery GamesRequest::getQuery() const
{
    if (id.empty() && name.empty())
        qWarning() << "Neither id nor name are specified.\n"
                      "Please check your usage of the API.";

    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("id"), id);
    addParam(query, QStringLiteral("name"), name);
    return query;
}

QUrlQuery StreamsRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("community_id"), communityId);
    addParam(query, QStringLiteral("game_id"), gameId);
    addParam(query, QStringLiteral("language"), language);
    addParam(query, QStringLiteral("user_id"), userId);
    addParam(query, QStringLiteral("user_login"), userLogin);
    return query;
}

QUrlQuery UsersRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("id"), id);
    addParam(query, QStringLiteral("login"), login);
    return query;
}

QUrlQuery UserFollowsRequest::getQuery() const
{
    if (!fromId && !toId)
        qWarning() << "Neither toId nor fromId is specified.\n"
                      "Please check your usage of the API.";

    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("from_id"), fromId);
    addParam(query, QStringLiteral("to_id"), toId);
    return query;
}

QUrlQuery VideosRequest::getQuery() const
{
    QUrlQuery query = Base::getQuery();
    addParam(query, QStringLiteral("id"), id);
    addParam(query, QStringLiteral("user_id"), userId);
    addParam(query, QStringLiteral("game_id"), gameId);
    addParam(query, QStringLiteral("language"), language);

    if (period) {
        const QString key(QStringLiteral("period"));
        switch (*period) {
        case Period::All:
            query.addQueryItem(key, QStringLiteral("all"));
            break;
        case Period::Day:
            query.addQueryItem(key, QStringLiteral("day"));
            break;
        case Period::Week:
            query.addQueryItem(key, QStringLiteral("week"));
            break;
        case Period::Month:
            query.addQueryItem(key, QStringLiteral("month"));
            break;
        }
    }

    if (sort) {
        const QString key(QStringLiteral("sort"));
        switch (*sort) {
        case Sorting::Time:
            query.addQueryItem(key, QStringLiteral("time"));
            break;
        case Sorting::Trending:
            query.addQueryItem(key, QStringLiteral("trending"));
            break;
        case Sorting::Views:
            query.addQueryItem(key, QStringLiteral("views"));
            break;
        }
    }

    if (type) {
        const QString key(QStringLiteral("type"));
        switch (*type) {
        case Type::All:
            query.addQueryItem(key, QStringLiteral("all"));
            break;
        case Type::Upload:
            query.addQueryItem(key, QStringLiteral("upload"));
            break;
        case Type::Archive:
            query.addQueryItem(key, QStringLiteral("archive"));
            break;
        case Type::Highlight:
            query.addQueryItem(key, QStringLiteral("highlight"));
            break;
        }
    }

    return query;
}

QNetworkRequest Helix::Request::getNetworkRequest(const std::optional<QString> &authorization) const
{
    auto result = QTwitch::Api::Request::getNetworkRequest(authorization);
    if (authorization)
        result.setRawHeader("Authorization", (QStringLiteral("Bearer ") + *authorization).toUtf8());
    return result;
}

GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(GamesRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(TopGamesRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(StreamsRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(UsersRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(UserFollowsRequest)
GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(VideosRequest)

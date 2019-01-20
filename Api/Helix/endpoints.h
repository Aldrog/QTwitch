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

#ifndef HELIX_ENDPOINTS_H
#define HELIX_ENDPOINTS_H

#include "request.h"
#include "data.h"
#include <QtDebug>

namespace QTwitch {
namespace Api {
namespace Helix {

class Request : public QTwitch::Api::Request
{
public:
    QString baseUrl() const final { return QStringLiteral("https://api.twitch.tv/helix/"); }
};

class PagedRequest : public Request
{
    typedef Request Base;
public:
    std::optional<QString> after;
    std::optional<QString> before;
    std::optional<int> first;

protected:
    QUrlQuery getQuery() const override;
};

class QTWITCHSHARED_EXPORT GamesRequest final : public Request
{
    typedef Request Base;
    typedef GamesList ResponseObjectType;
public:
    std::vector<int> id;
    std::vector<QString> name;

    QString endpoint() const final { return QStringLiteral("games"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

class QTWITCHSHARED_EXPORT TopGamesRequest final : public PagedRequest
{
    typedef PagedRequest Base;
    typedef GamesList ResponseObjectType;
public:
    QString endpoint() const final { return QStringLiteral("games/top"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;
};

class QTWITCHSHARED_EXPORT StreamsRequest final : public PagedRequest
{
    typedef PagedRequest Base;
    typedef StreamsList ResponseObjectType;
public:
    std::vector<QString> communityId;
    std::vector<QString> gameId;
    std::vector<QString> language;
    std::vector<QString> userId;
    std::vector<QString> userLogin;

    QString endpoint() const final { return QStringLiteral("streams"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

class QTWITCHSHARED_EXPORT UsersRequest final : public Request
{
    typedef Request Base;
    typedef UsersList ResponseObjectType;
public:
    std::vector<QString> id;
    std::vector<QString> login;

    QString endpoint() const final { return QStringLiteral("users"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

// TODO: According to docs this request doesn't allow backward navigation
class QTWITCHSHARED_EXPORT UserFollowsRequest final : public PagedRequest
{
    typedef PagedRequest Base;
    typedef FollowsList ResponseObjectType;
public:
    std::optional<QString> fromId;
    std::optional<QString> toId;

    QString endpoint() const final { return QStringLiteral("users/follows"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

class QTWITCHSHARED_EXPORT VideosRequest final : public PagedRequest
{
    typedef PagedRequest Base;
    typedef VideosList ResponseObjectType;
public:
    std::vector<QString> id;
    std::optional<QString> userId;
    std::optional<QString> gameId;
    std::optional<QString> language;

    enum class Period {
        All, Day, Week, Month
    };
    enum class Sorting {
        Time, Trending, Views
    };
    enum class Type {
        All, Upload, Archive, Highlight
    };

    std::optional<Period> period;
    std::optional<Sorting> sort;
    std::optional<Type> type;

    QString endpoint() const final { return QStringLiteral("videos"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const final;
};

}
}
}

#endif // HELIX_ENDPOINTS_H

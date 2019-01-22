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

#include "request.h"
#include <QUrlQuery>

using namespace QTwitch::Api;

QString Request::getUrlString() const
{
    return baseUrl() + endpoint();
}

QUrlQuery Request::getQuery() const
{
    return QUrlQuery();
}

QUrl Request::getFullUrl() const
{
    QUrl url(getUrlString());
    url.setQuery(getQuery());
    return url;
}

void Request::addParam(QUrlQuery &query, const QString &key, const QString &value) const
{
    query.addQueryItem(key, value);
}

void Request::addParam(QUrlQuery &query, const QString &key, const int &value) const
{
    query.addQueryItem(key, QString::number(value));
}

void Request::addParam(QUrlQuery &query, const QString &key, const bool &value) const
{
    query.addQueryItem(key, value ? QStringLiteral("true") : QStringLiteral("false"));
}

void Request::addParam(QUrlQuery &query, const QString &key, const std::vector<QString> &value) const
{
    if (value.empty())
        return;
    for (const auto &v: value)
        query.addQueryItem(key, v);
}

void Request::addParam(QUrlQuery &query, const QString &key, const std::vector<int> &value) const
{
    if (value.empty())
        return;
    for (auto v : value)
        query.addQueryItem(key, QString::number(v));
}

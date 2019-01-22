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

#ifndef REQUEST_H
#define REQUEST_H

#include "qtwitch_global.h"
#include <QNetworkReply>

namespace QTwitch {
namespace Api {

class Object;
struct Response;

class QTWITCHSHARED_EXPORT Request : public QObject
{
    Q_OBJECT
public:
    virtual ~Request() = default;
    virtual QString baseUrl() const = 0;
    virtual QString endpoint() const = 0;
    // Prefix for Authorization header
    virtual std::optional<QString> authorizationPrefix() const = 0;
    // When authorizationPrefix() is not set, setAuthorization will be called instead
    virtual void setAuthorization(const QString &) {}

    QUrl getFullUrl() const;
    virtual std::unique_ptr<Object> createResponseObject(const QByteArray &) const = 0;

    enum class RequestType {
        Get,
        Put,
        Delete
    };

    virtual RequestType requestType() const { return RequestType::Get; }

signals:
    void responseReceived(const std::shared_ptr<QTwitch::Api::Response> &request);
    void error(QNetworkReply::NetworkError e);

protected:
    virtual QUrlQuery getQuery() const;
    void addParam(QUrlQuery &query, const QString &key, const QString &value) const;
    void addParam(QUrlQuery &query, const QString &key, const int &value) const;
    void addParam(QUrlQuery &query, const QString &key, const bool &value) const;
    void addParam(QUrlQuery &query, const QString &key, const std::vector<QString> &value) const;
    void addParam(QUrlQuery &query, const QString &key, const std::vector<int> &value) const;

    template<typename T>
    void addParam(QUrlQuery &query, const QString &key, const std::optional<T> &value) const
    {
        if (!value)
            return;
        addParam(query, key, *value);
    }

private:
    QString getUrlString() const;
};

}
}

#endif // REQUEST_H

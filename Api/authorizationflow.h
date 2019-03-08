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

#ifndef AUTHORIZATIONFLOW_H
#define AUTHORIZATIONFLOW_H

#include "qtwitch_global.h"
#include "request.h"
#include "response.h"

#include <QObject>

namespace QTwitch {
namespace Api {
namespace Authorization {

class ValidationData : public Object
{
public:
    QString clientId;
    QString login;
    std::vector<QString> scopes;
    QString userId;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(clientId, "client_id");
        visitor.visit(login, "login");
        visitor.visit(scopes, "scopes");
        visitor.visit(userId, "user_id");
    }
};

class ValidateRequest final : public Request
{
    typedef Request Base;
    typedef ValidationData ResponseObjectType;
public:
    QString baseUrl() const override { return QStringLiteral("https://id.twitch.tv/oauth2/"); }
    QString endpoint() const override { return QStringLiteral("validate"); }
    QNetworkRequest getNetworkRequest(const std::optional<QString> &authorization) const final;
    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const override;
};

class RevokeRequest final : public Request
{
    typedef Request Base;
    typedef Object ResponseObjectType;
public:
    QString baseUrl() const override { return QStringLiteral("https://id.twitch.tv/oauth2/"); }
    QString endpoint() const override { return QStringLiteral("revoke"); }
    QNetworkRequest getNetworkRequest(const std::optional<QString> &authorization) const final;
    std::unique_ptr<Object> createResponseObject(const QByteArray &) const override { return std::make_unique<Object>(); }
    RequestType requestType() const final { return RequestType::Post; }
    QByteArray postData(const std::optional<QString> &authorization) const final;
};

}
}
}

#endif // AUTHORIZATIONFLOW_H

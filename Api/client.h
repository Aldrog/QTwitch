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

#ifndef CLIENT_H
#define CLIENT_H

#include "qtwitch_global.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <memory>

#include "request.h"
#include "response.h"

namespace QTwitch {
namespace Api {

class QTWITCHSHARED_EXPORT Client : public QObject
{
Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

public slots:
    void send(const std::shared_ptr<QTwitch::Api::Request> &request);

signals:
    void receive(const std::shared_ptr<QTwitch::Api::Response> &response);

    void error(QNetworkReply::NetworkError e);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &e);

private:
    std::unique_ptr<QNetworkAccessManager> network;
};

}
}

#endif // CLIENT_H

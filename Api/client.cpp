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

#include "client.h"
#include <QtDebug>
#include <QJsonDocument>
#include <memory>

using namespace QTwitch::Api;

Client::Client(QObject *parent) :
    QObject(parent),
    network(std::make_unique<QNetworkAccessManager>())
{
    connect(network.get(), &QNetworkAccessManager::sslErrors, this, &Client::sslErrors);
}

void Client::send(const std::shared_ptr<Request> &request)
{
    QNetworkRequest qreq(request->getFullUrl());
    qreq.setRawHeader("Client-ID", TWITCH_CLIENT_ID);
    QNetworkReply *qrep = network->get(qreq);
    connect(qrep, &QNetworkReply::readyRead, [&] ()
    {
        auto responseObject = request->createResponseObject(qrep->readAll());
        auto response = std::make_shared<Response>();
        response->object = std::move(responseObject);
        response->request = request;
        emit receive(response);
        emit request->responseReceived(response);
        qrep->deleteLater();
    });
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Client::error);
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), request.get(), &Request::error);
}

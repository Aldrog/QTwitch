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
#include "authorizationflow.h"

#include <QtDebug>
#include <QJsonDocument>
#include <QUrlQuery>
#include <QRandomGenerator>

using namespace QTwitch::Api;

Client::Client(QObject *parent)
    : QObject(parent)
    , network(std::make_unique<QNetworkAccessManager>())
    , mAuthorization(std::make_unique<AuthorizationManager>())
{
    connect(network.get(), &QNetworkAccessManager::sslErrors, this, &Client::sslErrors);
}

std::shared_ptr<Client> Client::get()
{
    static std::shared_ptr<Client> client;
    if (!client)
        client = std::make_shared<Client>();
    return client;
}

void Client::send(const std::shared_ptr<Request> &request)
{
    QNetworkRequest qreq = mAuthorization->authorize(request);

    QNetworkReply *qrep = nullptr;
    switch (request->requestType()) {
    case Request::RequestType::Get:
        qrep = network->get(qreq);
        break;
    case Request::RequestType::Put:
        qrep = network->put(qreq, QByteArray());
        break;
    case Request::RequestType::Delete:
        qrep = network->deleteResource(qreq);
        break;
    case Request::RequestType::Post:
        qrep = network->post(qreq, mAuthorization->authorizePost(request));
        break;
    }
    assert(qrep);

    connect(qrep, &QNetworkReply::finished, [this, qrep, request] ()
    {
        auto data = qrep->readAll();
        qDebug() << data;
        auto responseObject = request->createResponseObject(data);
        auto response = std::make_shared<Response>();

        response->object = std::move(responseObject);
        response->request = request;
        response->status = qrep->error();
        if (response->status != QNetworkReply::NoError)
            qInfo() << "Error:" << qrep->errorString();

        emit receive(response);
        emit request->responseReceived(response);
        qrep->deleteLater();
    });
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Client::error);
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), request.get(), &Request::error);
}

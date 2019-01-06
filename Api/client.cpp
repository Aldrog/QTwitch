#include "client.h"
#include <QtDebug>
#include <QJsonDocument>
#include <memory>

using namespace QTwitch::Api;

Client::Client(QObject *parent) :
    QObject(parent),
    network(std::make_unique<QNetworkAccessManager>())
{
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
        qrep->deleteLater();
    });
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Client::error);
    connect(qrep, &QNetworkReply::sslErrors, this, &Client::sslErrors);
}

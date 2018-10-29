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
    QNetworkRequest qreq(QString::fromStdString(request->getFullUrl({})));
    qreq.setRawHeader("Client-ID", TWITCH_CLIENT_ID);
    connect(network.get(), &QNetworkAccessManager::finished, [&] (QNetworkReply *qrep)
    {
        using namespace std::placeholders;
        auto responseObject = request->createResponseObject(QJsonDocument::fromJson(qrep->readAll()).object());
        auto response = std::make_shared<Response>();
        response->object = std::move(responseObject);
        response->request = request;
        emit receive(response);
        qrep->deleteLater();
    });
    QNetworkReply *qrep = network->get(qreq);
    connect(qrep, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &Client::error);
    connect(qrep, &QNetworkReply::sslErrors, this, &Client::sslErrors);
}

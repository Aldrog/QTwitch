#include "client.h"
#include <QtDebug>
#include <memory>

using namespace QTwitch::Api;

Client::Client(QObject *parent) :
    QObject(parent),
    network(std::make_unique<QNetworkAccessManager>())
{
}

void Client::send(const Request &request)
{
    Q_UNUSED(request);
    Q_UNIMPLEMENTED();
}

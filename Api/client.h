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

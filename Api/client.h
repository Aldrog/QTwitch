#ifndef CLIENT_H
#define CLIENT_H

#include "qtwitch_global.h"
#include <QObject>
#include <QNetworkAccessManager>
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
    void send(const Request &request);

signals:
    void receive(Response &response);

private:
    std::unique_ptr<QNetworkAccessManager> network;
};

}
}

#endif // CLIENT_H

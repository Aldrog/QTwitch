#ifndef REQUEST_H
#define REQUEST_H

#include "qtwitch_global.h"
#include <QNetworkReply>

namespace QTwitch {
namespace Api {

class Object;
class Response;

class QTWITCHSHARED_EXPORT Request : public QObject
{
    Q_OBJECT
public:
    virtual ~Request() = default;
    virtual QString baseUrl() const = 0;
    virtual QString endpoint() const = 0;

    virtual QUrl getFullUrl() const;
    virtual std::unique_ptr<Object> createResponseObject(const QByteArray &) const = 0;

signals:
    void responseReceived(const std::shared_ptr<QTwitch::Api::Response> &request);
    void error(QNetworkReply::NetworkError e);
};

}
}

#endif // REQUEST_H

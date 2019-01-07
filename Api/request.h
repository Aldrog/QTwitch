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

    QUrl getFullUrl() const;
    virtual std::unique_ptr<Object> createResponseObject(const QByteArray &) const = 0;

signals:
    void responseReceived(const std::shared_ptr<QTwitch::Api::Response> &request);
    void error(QNetworkReply::NetworkError e);

protected:
    virtual QUrlQuery getQuery() const;
    void addParam(QUrlQuery &query, const QString &key, const QString &value) const;
    void addParam(QUrlQuery &query, const QString &key, int value) const;
    void addParam(QUrlQuery &query, const QString &key, const std::vector<QString> &value) const;
    void addParam(QUrlQuery &query, const QString &key, const std::vector<int> &value) const;

private:
    QString getUrlString() const;
};

}
}

#endif // REQUEST_H

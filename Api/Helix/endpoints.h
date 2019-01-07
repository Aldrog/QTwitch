#ifndef HELIX_ENDPOINTS_H
#define HELIX_ENDPOINTS_H

#include "request.h"
#include "data.h"
#include "jsonsetter.h"
#include <QJsonDocument>
#include <QUrlQuery>
#include <QtDebug>

namespace QTwitch {
namespace Api {
namespace Helix {

class Request : public QTwitch::Api::Request
{
public:
    QString baseUrl() const final { return QStringLiteral("https://api.twitch.tv/helix/"); }
};

class PagedRequest : public Request
{
    typedef Request Base;
public:
    std::optional<QString> after;
    std::optional<QString> before;
    std::optional<int> first;

protected:
    QUrlQuery getQuery() const override;
};

class QTWITCHSHARED_EXPORT GamesRequest : public Request
{
    typedef Request Base;
public:
    std::vector<int> id;
    std::vector<QString> name;

    QString endpoint() const final { return QStringLiteral("games"); }
    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;

protected:
    QUrlQuery getQuery() const override;
};

class QTWITCHSHARED_EXPORT TopGamesRequest : public PagedRequest
{
public:
    QString endpoint() const final { return QStringLiteral("games/top"); }
    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final;
};

}
}
}

#endif // HELIX_ENDPOINTS_H

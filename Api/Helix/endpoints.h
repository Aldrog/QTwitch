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
    QUrlQuery getQuery() const override
    {
        if (after && before)
            qWarning() << "Both after and before paging parameters are set.\n"
                          "Please check your usage of the API.";
        if (first && (*first <= 0 || *first > 100))
            qWarning() << "Paging parameter \"first\" should be more than zero and not more than 100.\n"
                          "Please check your usage of the API.";

        QUrlQuery query = Base::getQuery();
        if (after)
            addParam(query, QStringLiteral("after"), *after);
        if (before)
            addParam(query, QStringLiteral("before"), *before);
        if (first)
            addParam(query, QStringLiteral("first"), QString::number(*first));
        return query;
    }
};

class QTWITCHSHARED_EXPORT GamesRequest : public Request
{
    typedef Request Base;
public:
    std::vector<int> id;
    std::vector<QString> name;

    QString endpoint() const final { return QStringLiteral("games"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final
    {
        typedef GamesList ObjectType;
        auto result = std::make_unique<ObjectType>();
        JsonSetter visitor(QJsonDocument::fromJson(data).object());
        result->accept(visitor);
        return result;
    }

protected:
    QUrlQuery getQuery() const override
    {
        if (id.empty() && name.empty())
            qWarning() << "Neither id nor name are specified.\n"
                          "Please check your usage of the API.";
        if (id.size() > 100)
            qWarning() << "More than 100 ids specified.\n"
                          "Please check your usage of the API.";
        if (name.size() > 100)
            qWarning() << "More than 100 names specified.\n"
                          "Please check your usage of the API.";

        QUrlQuery query = Base::getQuery();
        if (!id.empty())
            addParam(query, QStringLiteral("id"), id);
        if (!name.empty())
            addParam(query, QStringLiteral("name"), name);
        return query;
    }
};

class QTWITCHSHARED_EXPORT TopGamesRequest : public PagedRequest
{
public:
    QString endpoint() const final { return QStringLiteral("games/top"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final
    {
        typedef GamesList ObjectType;
        auto result = std::make_unique<ObjectType>();
        JsonSetter visitor(QJsonDocument::fromJson(data).object());
        result->accept(visitor);
        return result;
    }
};

}
}
}

#endif // HELIX_ENDPOINTS_H

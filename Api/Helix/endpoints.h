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
public:
    std::optional<QString> after;
    std::optional<QString> before;
    std::optional<int> first;

    QUrl getFullUrl() const override
    {
        if (after && before)
            qWarning() << "Both after and before paging parameters are set.\n"
                          "Please check your usage of the API.";
        if (first && (*first <= 0 || *first > 100))
            qWarning() << "Paging parameter \"first\" should be more than zero and not more than 100.\n"
                          "Please check your usage of the API.";

        auto url = QTwitch::Api::Request::getFullUrl();
        QUrlQuery query(url);
        if (after)
            query.addQueryItem(QStringLiteral("after"), *after);
        if (before)
            query.addQueryItem(QStringLiteral("before"), *before);
        if (first)
            query.addQueryItem(QStringLiteral("first"), QString::number(*first));
        url.setQuery(query);
        return url;
    }
};

class QTWITCHSHARED_EXPORT TopGamesRequest : public PagedRequest
{
public:
    QString endpoint() const final { return QStringLiteral("games/top"); }

    std::unique_ptr<Object> createResponseObject(const QByteArray &data) const final
    {
        typedef TopGames ObjectType;
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

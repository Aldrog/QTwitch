#include "endpoints.h"

using namespace QTwitch::Api;
using namespace QTwitch::Api::Helix;

QUrlQuery PagedRequest::getQuery() const
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

QUrlQuery GamesRequest::getQuery() const
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

#define CONSTRUCT_RESULT_FROM_JSON \
    auto result = std::make_unique<ObjectType>(); \
    JsonSetter visitor(QJsonDocument::fromJson(data).object()); \
    result->accept(visitor);

std::unique_ptr<Object> GamesRequest::createResponseObject(const QByteArray &data) const
{
    typedef GamesList ObjectType;
    CONSTRUCT_RESULT_FROM_JSON
    return result;
}

std::unique_ptr<Object> TopGamesRequest::createResponseObject(const QByteArray &data) const
{
    typedef GamesList ObjectType;
    CONSTRUCT_RESULT_FROM_JSON
    return result;
}


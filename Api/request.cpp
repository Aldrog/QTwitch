#include "request.h"
#include <QUrlQuery>

using namespace QTwitch::Api;

QString Request::getUrlString() const
{
    return baseUrl() + endpoint();
}

QUrlQuery Request::getQuery() const
{
    return QUrlQuery();
}

QUrl Request::getFullUrl() const
{
    QUrl url(getUrlString());
    url.setQuery(getQuery());
    return url;
}

void Request::addParam(QUrlQuery &query, const QString &key, const std::optional<QString> &value) const
{
    if (!value)
        return;
    query.addQueryItem(key, *value);
}

void Request::addParam(QUrlQuery &query, const QString &key, const std::optional<int> &value) const
{
    if (!value)
        return;
    query.addQueryItem(key, QString::number(*value));
}

void Request::addParam(QUrlQuery &query, const QString &key, const std::vector<QString> &value) const
{
    if (value.empty())
        return;
    auto it = value.begin();
    QString valString = *it;
    for (++it; it != value.end(); ++it)
        valString += ',' + *it;
    query.addQueryItem(key, valString);
}

void Request::addParam(QUrlQuery &query, const QString &key, const std::vector<int> &value) const
{
    if (value.empty())
        return;
    auto it = value.begin();
    QString valString = QString::number(*it);
    for (++it; it != value.end(); ++it)
        valString += ',' + QString::number(*it);
    query.addQueryItem(key, valString);
}

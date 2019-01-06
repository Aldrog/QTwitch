#include "request.h"
#include <QUrl>

using namespace QTwitch::Api;

QUrl Request::getFullUrl() const
{
    return QUrl(baseUrl() + endpoint());
}

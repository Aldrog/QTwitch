#ifndef REQUEST_H
#define REQUEST_H

#include "qtwitch_global.h"

namespace QTwitch {
namespace Api {

class Object;

class QTWITCHSHARED_EXPORT Request
{
public:
    virtual ~Request() = default;
    virtual QString baseUrl() const = 0;
    virtual QString endpoint() const = 0;
//    virtual std::vector<std::string> options() const = 0;

    virtual QUrl getFullUrl() const;
    virtual std::unique_ptr<Object> createResponseObject(const QByteArray &) const = 0;
};

}
}

#endif // REQUEST_H

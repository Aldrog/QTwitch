#ifndef REQUEST_H
#define REQUEST_H

#include "qtwitch_global.h"

#include <string>
#include <vector>
#include <memory>

namespace QTwitch {
namespace Api {

class Object;

struct QTWITCHSHARED_EXPORT Request
{
    typedef std::pair<std::string, std::string> Argument;

    virtual ~Request() = default;
    virtual const std::string baseUrl() const = 0;
    virtual const std::string endpoint() const = 0;
    virtual const std::vector<std::string> options() const = 0;

    virtual std::unique_ptr<Object> createResponseObject(const QJsonObject &) const = 0;
    std::string getFullUrl(std::initializer_list< Argument > arguments) const;
};

}
}

#endif // REQUEST_H

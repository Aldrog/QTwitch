#ifndef RESPONSE_H
#define RESPONSE_H

#include "qtwitch_global.h"
#include "request.h"

#include <QVariant>
#include <QJsonObject>
#include <QMetaProperty>
#include <memory>

namespace QTwitch {
namespace Api {

class Object
{
public:
    Object(const Object &) = delete;
    Object& operator =(const Object &) = delete;

    Object() = default;
    Object(Object &&) = default;
    Object& operator =(Object &&) = default;
    virtual ~Object() = default;
};

struct QTWITCHSHARED_EXPORT Response
{
    std::shared_ptr<Request> request;
    std::unique_ptr<Object> object;
};

}
}

Q_DECLARE_METATYPE(std::shared_ptr<QTwitch::Api::Response>)

#endif // RESPONSE_H

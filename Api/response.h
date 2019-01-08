/*
 * Copyright © 2019 Andrew Penkrat <contact.aldrog@gmail.com>
 *
 * This file is part of QTwitch.
 *
 * QTwitch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QTwitch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with QTwitch.  If not, see <https://www.gnu.org/licenses/>.
 */

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

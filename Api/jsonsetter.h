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

#ifndef JSONSETTER_H
#define JSONSETTER_H

#include <vector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace QTwitch {
namespace Api {

class JsonSetter
{
public:
    explicit JsonSetter(const QJsonObject &input)
        : json(input)
    { }

    template<class T>
    void visit(T &value, const QString &key) const
    {
        set(value, json[key]);
    }

    template<class T>
    void visit(std::vector<T> &object, const QString &key) const
    {
        const auto jsonArray = json[key].toArray();
        for (const auto &it : jsonArray)
        {
            T el;
            set(el, it);
            object.push_back(std::move(el));
        }
    }
private:
    QJsonObject json;

    template<class T>
    inline void set(T &value, const QJsonValue &it) const { value.accept(JsonSetter(it.toObject())); }
    inline void set(QString &value, const QJsonValue &it) const { value = it.toString(); }
    inline void set(int &value, const QJsonValue &it) const { value = it.toInt(); }
    inline void set(double &value, const QJsonValue &it) const { value = it.toDouble(); }
    inline void set(bool &value, const QJsonValue &it) const { value = it.toBool(); }
};

}
}

#define GENERATE_JSON_TO_OBJECT_CONSTRUCTOR(RequestType) \
    std::unique_ptr<Object> RequestType::createResponseObject(const QByteArray &data) const \
    { \
        auto result = std::make_unique<ResponseObjectType>(); \
        JsonSetter visitor(QJsonDocument::fromJson(data).object()); \
        result->accept(visitor); \
        return std::move(result); \
    }

#endif // JSONSETTER_H

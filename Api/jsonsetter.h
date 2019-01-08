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

namespace QTwitch {
namespace Api {

template<typename C, typename Arg>
struct supportsVisitors
{
private:
    template<typename T>
    static constexpr auto check(T *x) -> decltype(x->accept(std::declval<Arg>()), std::true_type{});

    template<typename>
    static constexpr auto check(...) -> std::false_type;

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

class JsonSetter
{
public:
    explicit JsonSetter(const QJsonObject &input);

    template<class T>
    void visit(T &object, const QString &key) const
    {
        object.accept(JsonSetter(json[key].toObject()));
    }

    template<class T>
    void visit(std::vector<T> &object, const QString &key) const
    {
        auto jsonArray = json[key].toArray();
        for (auto it : jsonArray)
        {
            T el;
            if constexpr (supportsVisitors<T, JsonSetter>::value)
                el.accept(JsonSetter(it.toObject()));
            else
                set(el, it);
            object.push_back(std::move(el));
        }
    }

    void visit(QString &value, const QString &key) const;
    void visit(int &value, const QString &key) const;
    void visit(double &value, const QString &key) const;
    void visit(bool &value, const QString &key) const;
private:
    QJsonObject json;

    inline void set(QString &value, const QJsonValueRef &it) const { value = it.toString(); }
    inline void set(int &value, const QJsonValueRef &it) const { value = it.toInt(); }
    inline void set(double &value, const QJsonValueRef &it) const { value = it.toDouble(); }
    inline void set(bool &value, const QJsonValueRef &it) const { value = it.toBool(); }
};

}
}

#endif // JSONSETTER_H

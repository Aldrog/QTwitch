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
    void visit(T &object, const std::string &key) const
    {
        object.accept(JsonSetter(json[QString::fromStdString(key)].toObject()));
    }

    template<class T>
    void visit(std::vector<T> &object, const std::string &key) const
    {
        auto jsonArray = json[QString::fromStdString(key)].toArray();
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

    void visit(std::string &value, const std::string &key) const;
    void visit(int &value, const std::string &key) const;
    void visit(double &value, const std::string &key) const;
    void visit(bool &value, const std::string &key) const;
private:
    QJsonObject json;

    inline void set(std::string &value, const QJsonValueRef &it) const { value = it.toString().toStdString(); }
    inline void set(int &value, const QJsonValueRef &it) const { value = it.toInt(); }
    inline void set(double &value, const QJsonValueRef &it) const { value = it.toDouble(); }
    inline void set(bool &value, const QJsonValueRef &it) const { value = it.toBool(); }
};

}
}

#endif // JSONSETTER_H

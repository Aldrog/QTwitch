#ifndef JSONSETTER_H
#define JSONSETTER_H

#include <vector>
#include <QJsonObject>
#include <QJsonArray>

class JsonSetter
{
public:
    explicit JsonSetter(const QJsonObject &input);

    template<class T>
    void visit(T &object, const std::string &key) const
    {
//        auto sub = JsonSetter(json[QString::fromStdString(key)].toObject());
        object.accept(JsonSetter(json[QString::fromStdString(key)].toObject()));
    }

    template<class T>
    void visit(std::vector<T> &object, const std::string &key) const
    {
        auto jsonArray = json[QString::fromStdString(key)].toArray();
        for (auto it : jsonArray)
        {
            T el;
            el.accept(JsonSetter(it.toObject()));
            object.push_back(std::move(el));
        }
    }

    void visit(std::string &value, const std::string &key) const;
    void visit(int &value, const std::string &key) const;
    void visit(double &value, const std::string &key) const;
    void visit(bool &value, const std::string &key) const;
private:
    QJsonObject json;
};

#endif // JSONSETTER_H

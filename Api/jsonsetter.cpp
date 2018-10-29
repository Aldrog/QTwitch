#include "jsonsetter.h"

JsonSetter::JsonSetter(const QJsonObject &input)
    : json(input)
{
}

void JsonSetter::visit(std::string &value, const std::string &key) const
{
    value = json[QString::fromStdString(key)].toString().toStdString();
}

void JsonSetter::visit(int &value, const std::string &key) const
{
    value = json[QString::fromStdString(key)].toInt();
}

void JsonSetter::visit(double &value, const std::string &key) const
{
    value = json[QString::fromStdString(key)].toDouble();
}

void JsonSetter::visit(bool &value, const std::string &key) const
{
    value = json[QString::fromStdString(key)].toBool();
}

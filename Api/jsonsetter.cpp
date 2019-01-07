#include "jsonsetter.h"

using namespace QTwitch::Api;

JsonSetter::JsonSetter(const QJsonObject &input)
    : json(input)
{
}

void JsonSetter::visit(QString &value, const QString &key) const
{
    value = json[key].toString();
}

void JsonSetter::visit(int &value, const QString &key) const
{
    value = json[key].toInt();
}

void JsonSetter::visit(double &value, const QString &key) const
{
    value = json[key].toDouble();
}

void JsonSetter::visit(bool &value, const QString &key) const
{
    value = json[key].toBool();
}

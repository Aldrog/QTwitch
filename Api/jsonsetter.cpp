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

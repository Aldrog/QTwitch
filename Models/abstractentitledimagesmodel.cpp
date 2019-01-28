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

#include "abstractentitledimagesmodel.h"

using namespace QTwitch::Models;

AbstractEntitledImagesModel::AbstractEntitledImagesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int AbstractEntitledImagesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return storageSize();
}

QHash<int, QByteArray> AbstractEntitledImagesModel::roleNames() const
{
    return QHash<int, QByteArray>({
                          { static_cast<int>(Role::Image),          "image" }
                        , { static_cast<int>(Role::Title),          "title" }
                        , { static_cast<int>(Role::AdditionalData), "additionalData" }
                                  });
}

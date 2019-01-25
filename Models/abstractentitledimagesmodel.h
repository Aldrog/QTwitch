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

#ifndef ABSTRACTENTITLEDIMAGESMODEL_H
#define ABSTRACTENTITLEDIMAGESMODEL_H

#include "qtwitch_global.h"
#include "entitledimage.h"

#include <QAbstractListModel>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT AbstractEntitledImagesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum class Role : int {
        Image = Qt::UserRole,
        Title,
        Subtitle,
        LastRole = Subtitle
    };

    explicit AbstractEntitledImagesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    virtual void next() = 0;
    virtual bool nextAvailable() = 0;
    virtual void reload() = 0;

protected:
    std::vector<EntitledImage> storage;
};

}
}

#endif // ABSTRACTENTITLEDIMAGESMODEL_H

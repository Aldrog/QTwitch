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
    Q_PROPERTY(int pageSize READ pageSize WRITE setPageSize NOTIFY pageSizeChanged RESET resetPageSize)
    // imageHeight is calculated from imageWidth
    // imageWidth and imageHeight may not reflect actual image sizes for some models
    Q_PROPERTY(int imageWidth MEMBER imageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(int imageHeight READ imageHeight NOTIFY imageHeightChanged)
public:
    enum class Role : int {
        Image = Qt::UserRole,
        Title,
        AdditionalData, // Additional structure of model-specific data
        LastRole = AdditionalData
    };

    explicit AbstractEntitledImagesModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;

    virtual int pageSize() const = 0;
    virtual void setPageSize(int newSize) = 0;
    virtual void resetPageSize() = 0;
    virtual int imageHeight() const = 0;

    Q_INVOKABLE virtual bool nextAvailable() const = 0;

    int rowCount(const QModelIndex &parent) const final;

    int imageWidth = 480;

public slots:
    virtual void next() = 0;
    virtual void reload() = 0;

signals:
    void pageSizeChanged(int newSize);
    void imageWidthChanged();
    void imageHeightChanged();

protected:
    virtual std::size_t storageSize() const = 0;
    virtual void resetStorage() = 0;
};

}
}

#endif // ABSTRACTENTITLEDIMAGESMODEL_H

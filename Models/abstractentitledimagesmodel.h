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
    Q_PROPERTY(bool nextAvailable READ nextAvailable NOTIFY nextAvailableChanged)
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
    virtual bool nextAvailable() const = 0;

    int imageWidth = 480;

public slots:
    virtual void next() = 0;
    virtual void reload() = 0;

signals:
    void pageSizeChanged(int pageSize);
    void imageWidthChanged();
    void imageHeightChanged();
    void nextAvailableChanged();
};

template <class PayloadType>
class EntitledImagesModel : public AbstractEntitledImagesModel
{
public:
    explicit EntitledImagesModel(QObject *parent = nullptr)
        : AbstractEntitledImagesModel(parent)
    {}

    int rowCount(const QModelIndex &parent) const final {
        if (parent.isValid())
            return 0;
        else
            return storage.size();
    }

    QVariant data(const QModelIndex &index, int role) const final
    {
        if (!index.isValid())
            return QVariant();

        if (index.row() < 0 || static_cast<unsigned int>(index.row()) >= storage.size())
            return QVariant();

        if (role < Qt::UserRole || role > static_cast<int>(Role::LastRole))
            return QVariant();

        switch (static_cast<Role>(role)) {
        case Role::Image:
            return storage[index.row()].img.imageUrl;
        case Role::Title:
            return storage[index.row()].img.title;
        case Role::AdditionalData:
            return QVariant::fromValue(storage[index.row()].payload);
        }

        return QVariant();
    }

protected:
    struct Data {
        Data(EntitledImage img_, PayloadType payload_)
            : img(std::move(img_)), payload(std::move(payload_))
        {}
        EntitledImage img;
        PayloadType payload;
    };

    std::vector<Data> storage;
};

}
}

#endif // ABSTRACTENTITLEDIMAGESMODEL_H

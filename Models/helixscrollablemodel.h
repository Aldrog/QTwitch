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

#ifndef HELIXSCROLLABLEMODEL_H
#define HELIXSCROLLABLEMODEL_H

#include "abstractentitledimagesmodel.h"
#include <Api/request.h>
#include <Api/client.h>

namespace QTwitch {
namespace Models {

template <class PayloadType, class RequestType>
class HelixScrollableModel : public EntitledImagesModel<PayloadType>
{
public:
    explicit HelixScrollableModel(QObject *parent = nullptr)
        : EntitledImagesModel<PayloadType>(parent)
        , request(std::make_shared<RequestType>())
    {
        this->connect(request.get(), &Api::Request::responseReceived, this, &HelixScrollableModel::receiveData);
    }

    int pageSize() const final
    {
        if (!request->first)
            return 0;
        return *request->first;
    }

    void setPageSize(int newSize) final
    {
        if (pageSize() != newSize) {
            request->first = newSize;
            emit this->pageSizeChanged(newSize);
        }
    }

    void resetPageSize() final
    {
        if (request->first) {
            request->first.reset();
            emit this->pageSizeChanged(0);
        }
    }

    bool nextAvailable() const final
    {
        return !pagingCursor.isEmpty();
    }

    void next() final
    {
        request->after = pagingCursor;
        request->before.reset();
        Api::Client::get()->send(request);
    }

    void reload() final
    {
        pagingCursor.clear();
        request->after.reset();
        request->before.reset();
        if (this->storage.size() != 0) {
            this->beginRemoveRows(QModelIndex(), 0, this->storage.size() - 1);
            this->storage.clear();
            this->endRemoveRows();
        }
        Api::Client::get()->send(request);
    }

protected:
    std::shared_ptr<RequestType> request;

    void updateCursor(const QString &cursor)
    {
        bool wasAvailable = nextAvailable();
        pagingCursor = cursor;
        if (nextAvailable() != wasAvailable)
            emit this->nextAvailableChanged();
    }

private:
    QString pagingCursor;

protected slots:
    virtual void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response) = 0;
};

}
}

#endif // HELIXSCROLLABLEMODEL_H

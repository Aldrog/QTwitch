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

#ifndef LEGACYSCROLLABLEMODEL_H
#define LEGACYSCROLLABLEMODEL_H

#include "abstractentitledimagesmodel.h"
#include <Api/request.h>
#include <Api/client.h>

namespace QTwitch {
namespace Models {

template <class PayloadType, class RequestType>
class LegacyScrollableModel : public EntitledImagesModel<PayloadType>
{
public:
    explicit LegacyScrollableModel(QObject *parent = nullptr)
        : EntitledImagesModel<PayloadType>(parent)
        , request(std::make_shared<RequestType>())
    {
        this->connect(request.get(), &Api::Request::responseReceived, this, &LegacyScrollableModel::receiveData);
    }

    int pageSize() const final
    {
        if (!request->limit)
            return 0;
        return *request->limit;
    }

    void setPageSize(int newSize) final
    {
        if (pageSize() != newSize) {
            request->limit = newSize;
            emit this->pageSizeChanged(newSize);
        }
    }

    void resetPageSize() final
    {
        request->limit.reset();
    }

    bool nextAvailable() const final
    {
        return this->storage.size() < totalCount;
    }

    void next() final
    {
        request->offset = this->storage.size();
        Api::Client::get()->send(request);
    }

    void reload() final
    {
        request->offset.reset();
        if (this->storage.size() != 0) {
            this->beginRemoveRows(QModelIndex(), 0, this->storage.size() - 1);
            this->storage.clear();
            this->endRemoveRows();
        }
        Api::Client::get()->send(request);
    }

protected:
    std::shared_ptr<RequestType> request;

    void updateTotal(int total)
    {
        totalCount = total;
    }

private:
    unsigned int totalCount = 0;

protected slots:
    virtual void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response) = 0;
};

}
}

#endif // LEGACYSCROLLABLEMODEL_H

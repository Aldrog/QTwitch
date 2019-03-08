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

#include "legacyscrollablemodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Models::Legacy;
using namespace QTwitch::Api;

LegacyScrollableModel::LegacyScrollableModel(QObject *parent)
    : AbstractEntitledImagesModel(parent)
{
}

void LegacyScrollableModel::next()
{
    auto request = getRequest();
    request->offset = storageSize();
    Client::get()->send(request);
}

bool LegacyScrollableModel::nextAvailable() const
{
    return storageSize() < totalCount;
}

void LegacyScrollableModel::reload()
{
    auto request = getRequest();
    request->offset.reset();
    if (storageSize() != 0) {
        beginRemoveRows(QModelIndex(), 0, storageSize() - 1);
        resetStorage();
        endRemoveRows();
    }
    Client::get()->send(request);
}

int LegacyScrollableModel::pageSize() const
{
    auto request = getRequest();
    if (!request->limit)
        return 0;
    return *request->limit;
}

void LegacyScrollableModel::setPageSize(int newSize)
{
    if (pageSize() != newSize) {
        getRequest()->limit = newSize;
        emit pageSizeChanged(newSize);
    }
}

void LegacyScrollableModel::resetPageSize()
{
    getRequest()->limit.reset();
}

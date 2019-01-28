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

#include "helixscrollablemodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;

HelixScrollableModel::HelixScrollableModel(QObject *parent)
    : AbstractEntitledImagesModel(parent)
{
}

void HelixScrollableModel::next()
{
    auto request = getRequest();
    request->after = pagingCursor;
    request->before.reset();
    QTwitch::Api::Client::getClient()->send(request);
}

bool HelixScrollableModel::nextAvailable() const
{
    return !pagingCursor.isEmpty();
}

void HelixScrollableModel::reload()
{
    auto request = getRequest();
    pagingCursor.clear();
    request->after.reset();
    request->before.reset();
    resetStorage();
    QTwitch::Api::Client::getClient()->send(request);
}

int HelixScrollableModel::pageSize() const
{
    auto request = getRequest();
    if (!request->first)
        return 0;
    return *request->first;
}

void HelixScrollableModel::setPageSize(int newSize)
{
    if (pageSize() != newSize) {
        getRequest()->first = newSize;
        emit pageSizeChanged(newSize);
    }
}

void HelixScrollableModel::resetPageSize()
{
    auto request = getRequest();
    if (request->first) {
        request->first.reset();
        emit pageSizeChanged(0);
    }
}

void HelixScrollableModel::updateCursor(const QString &cursor)
{
    pagingCursor = cursor;
}

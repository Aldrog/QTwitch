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

LegacyScrollableModel::LegacyScrollableModel(QObject *parent)
    : AbstractEntitledImagesModel(parent)
{
}

void LegacyScrollableModel::next()
{
    auto request = getRequest();
    request->offset = storage.size();
    QTwitch::Api::Client::getClient()->send(request);
}

bool LegacyScrollableModel::nextAvailable() const
{
    return storage.size() < totalCount;
}

void LegacyScrollableModel::reload()
{
    auto request = getRequest();
    request->offset.reset();
    storage.clear();
    QTwitch::Api::Client::getClient()->send(request);
}

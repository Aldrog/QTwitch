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

#include "userssearchmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

UsersSearchModel::UsersSearchModel(QObject *parent)
    : LegacyScrollableModel(parent)
{ }

void UsersSearchModel::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<v5::ChannelsList>(static_cast<v5::ChannelsList*>(response->object.release()));
    beginInsertRows(QModelIndex(), storage.size(), storage.size() + data->channels.size() - 1);
    for (const auto &channel : data->channels) {
        storage.emplace_back( EntitledImage(channel.logo, channel.displayName),
                              UserPayload(QString::number(channel.id)) );
    }
    endInsertRows();
    updateTotal(data->total);
}

QString UsersSearchModel::query() const
{
    return request->searchQuery;
}

void UsersSearchModel::setQuery(const QString &newQuery)
{
    if (newQuery != request->searchQuery) {
        request->searchQuery = newQuery;
        emit queryChanged(newQuery);
    }
}

void UsersSearchModel::resetQuery()
{
    if (!request->searchQuery.isEmpty()) {
        request->searchQuery.clear();
        emit queryChanged(QString());
    }
}

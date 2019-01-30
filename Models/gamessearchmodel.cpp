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

#include "gamessearchmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Models::Legacy;
using namespace QTwitch::Api;

GamesSearchModel::GamesSearchModel(QObject *parent)
    : AbstractEntitledImagesModel(parent)
{
    request = std::make_shared<v5::SearchGamesRequest>();
    connect(request.get(), &Request::responseReceived, this, &GamesSearchModel::receiveData);
    reload();
}

QVariant GamesSearchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
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

void GamesSearchModel::reload()
{
    resetStorage();
    Client::getClient()->send(request);
}

void GamesSearchModel::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<v5::GamesList>(static_cast<v5::GamesList*>(response->object.release()));
    beginInsertRows(QModelIndex(), storageSize(), storageSize() + data->games.size() - 1);
    for (const auto &game : data->games) {
        storage.emplace_back( EntitledImage(game.logo.largeUrl, game.name),
                              GamePayload(QString::number(game.id)) );
    }
    endInsertRows();
}

QString GamesSearchModel::query() const
{
    return request->searchQuery;
}

void GamesSearchModel::setQuery(const QString &newQuery)
{
    if (newQuery != request->searchQuery) {
        request->searchQuery = newQuery;
        emit queryChanged(newQuery);
    }
}

void GamesSearchModel::resetQuery()
{
    if (!request->searchQuery.isEmpty()) {
        request->searchQuery.clear();
        emit queryChanged(QString());
    }
}

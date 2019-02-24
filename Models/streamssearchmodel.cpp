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

#include "streamssearchmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Models::Legacy;
using namespace QTwitch::Api;

StreamsSearchModel::StreamsSearchModel(QObject *parent)
    : LegacyScrollableModel(parent)
{
    request = std::make_shared<v5::SearchStreamsRequest>();
    connect(request.get(), &Request::responseReceived, this, &StreamsSearchModel::receiveData);
}

QVariant StreamsSearchModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || static_cast<unsigned int>(index.row()) >= storageSize())
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

void StreamsSearchModel::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<v5::StreamsList>(static_cast<v5::StreamsList*>(response->object.release()));
    updateTotal(data->total);
    beginInsertRows(QModelIndex(), storageSize(), storageSize() + data->streams.size() - 1);
    for (const auto &stream : data->streams) {
        QString imgUrl = stream.preview.templateUrl;
        imgUrl.replace(QStringLiteral("{width}"), QString::number(imageWidth));
        imgUrl.replace(QStringLiteral("{height}"), QString::number(imageHeight));
        storage.emplace_back( EntitledImage(imgUrl, stream.channel.displayName),
                              StreamPayload(stream.channel.status, stream.viewers, QString::number(stream.channel.id)) );
    }
    endInsertRows();
}

QString StreamsSearchModel::query() const
{
    return request->searchQuery;
}

void StreamsSearchModel::setQuery(const QString &newQuery)
{
    if (newQuery != request->searchQuery) {
        request->searchQuery = newQuery;
        emit queryChanged(newQuery);
    }
}

void StreamsSearchModel::resetQuery()
{
    if (!request->searchQuery.isEmpty()) {
        request->searchQuery.clear();
        emit queryChanged(QString());
    }
}

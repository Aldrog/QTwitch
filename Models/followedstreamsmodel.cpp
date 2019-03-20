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

#include "followedstreamsmodel.h"

using namespace QTwitch::Models;
using namespace QTwitch::Api;

FollowedStreamsModel::FollowedStreamsModel(QObject *parent)
    : LegacyScrollableModel(parent)
{
}

void FollowedStreamsModel::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<v5::StreamsList>(static_cast<v5::StreamsList*>(response->object.release()));
    beginInsertRows(QModelIndex(), storage.size(), storage.size() + data->streams.size() - 1);
    for (const auto &stream : data->streams) {
        QString imgUrl = stream.preview.templateUrl;
        imgUrl.replace(QStringLiteral("{width}"), QString::number(imageWidth));
        imgUrl.replace(QStringLiteral("{height}"), QString::number(imageHeight()));
        storage.emplace_back( EntitledImage(imgUrl, stream.channel.displayName),
                              StreamPayload(stream.channel.status, stream.viewers, QString::number(stream.channel.id)) );
    }
    endInsertRows();
    updateTotal(data->total);
}

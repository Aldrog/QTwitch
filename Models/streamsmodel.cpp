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

#include "streamsmodel.h"

using namespace QTwitch::Models;
using namespace QTwitch::Api;

StreamsModel::StreamsModel(QObject *parent)
    : HelixScrollableModel(parent)
{
}

QStringList StreamsModel::gameFilter() const
{
    QStringList result;
    for (const auto &i : request->gameId)
        result.append(i);
    return result;
}

QStringList StreamsModel::languageFilter() const
{
    QStringList result;
    for (const auto &i : request->language)
        result.append(i);
    return result;
}

void StreamsModel::setGameFilter(const QStringList &newGameFilter)
{
    request->gameId.clear();
    request->gameId.reserve(newGameFilter.size());
    for (const auto &i : newGameFilter)
        request->gameId.push_back(i);
    emit gameFilterChanged(newGameFilter);
}

void StreamsModel::setLanguageFilter(const QStringList &newLanguageFilter)
{
    request->language.clear();
    request->language.reserve(newLanguageFilter.size());
    for (const auto &i : newLanguageFilter)
        request->language.push_back(i);
    emit languageFilterChanged(newLanguageFilter);
}

void StreamsModel::resetGameFilter()
{
    if (!request->gameId.empty()) {
        request->gameId.clear();
        emit gameFilterChanged({});
    }
}

void StreamsModel::resetLanguageFilter()
{
    if (!request->language.empty()) {
        request->language.clear();
        emit languageFilterChanged({});
    }
}

void StreamsModel::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<Helix::StreamsList>(static_cast<Helix::StreamsList*>(response->object.release()));
    updateCursor(data->pagination.cursor);
    beginInsertRows(QModelIndex(), storage.size(), storage.size() + data->data.size() - 1);
    for (const auto &stream : data->data) {
        QString imgUrl = stream.thumbnailUrl;
        imgUrl.replace(QStringLiteral("{width}"), QString::number(imageWidth));
        imgUrl.replace(QStringLiteral("{height}"), QString::number(imageHeight()));
        storage.emplace_back( EntitledImage(imgUrl, stream.userName),
                              StreamPayload(stream.title, stream.viewerCount, stream.userId) );
    }
    endInsertRows();
}

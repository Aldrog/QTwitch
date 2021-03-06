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

#include "followedgamesmodel.h"

using namespace QTwitch::Models;
using namespace QTwitch::Api;

FollowedGamesModel::FollowedGamesModel(QObject *parent)
    : LegacyScrollableModel(parent)
{
    request->userLogin = Client::get()->authorization()->login();
}

void FollowedGamesModel::receiveData(const std::shared_ptr<QTwitch::Api::Response> &response)
{
    bool wasAvailable = nextAvailable();
    auto data = std::unique_ptr<Experimental::FollowedGamesList>(static_cast<Experimental::FollowedGamesList*>(response->object.release()));
    updateTotal(data->total);
    beginInsertRows(QModelIndex(), storage.size(), storage.size() + data->follows.size() - 1);
    for (const auto &gameData : data->follows) {
        const auto &game = gameData.game;
        QString imgUrl = game.box.templateUrl;
        imgUrl.replace(QStringLiteral("{width}"), QString::number(imageWidth));
        imgUrl.replace(QStringLiteral("{height}"), QString::number(imageHeight()));
        storage.emplace_back( EntitledImage(imgUrl, game.name),
                              GamePayload(QString::number(game.id)) );
    }
    endInsertRows();
    if (nextAvailable() != wasAvailable)
        emit nextAvailableChanged();
}

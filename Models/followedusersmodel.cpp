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

#include "followedusersmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

FollowedUsersModel::FollowedUsersModel(QObject *parent)
    : HelixScrollableModel(parent)
{
    request->fromId = Client::get()->authorization()->userId();

    usersRequest = std::make_shared<Helix::UsersRequest>();
    connect(usersRequest.get(), &Request::responseReceived, this, &FollowedUsersModel::receiveUsers);
}

void FollowedUsersModel::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<Helix::FollowsList>(static_cast<Helix::FollowsList*>(response->object.release()));
    usersRequest->id.clear();
    for (const auto &follow : data->data) {
        usersRequest->id.push_back(follow.toId);
    }
    auto client = Client::get();
    client->send(usersRequest);
    updateCursor(data->pagination.cursor);
}

void FollowedUsersModel::receiveUsers(const std::shared_ptr<Response> &response)
{
    usersResponse = response;
    pushData();
}

void FollowedUsersModel::pushData()
{
    auto users = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(usersResponse->object.release()));
    usersResponse.reset();
    beginInsertRows(QModelIndex(), storage.size(), storage.size() + users->data.size() - 1);
    for (const auto &user : users->data) {
        storage.emplace_back( EntitledImage(user.profileImageUrl, user.displayName),
                              UserPayload(user.id) );
    }
    endInsertRows();
}

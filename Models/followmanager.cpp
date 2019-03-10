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

#include "followmanager.h"
#include <Api/Helix/endpoints.h>
#include <Api/v5/endpoints.h>
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

FollowManager::FollowManager(QObject *parent)
    : QObject(parent)
{
}

void FollowManager::setTargetUser(const QString &newTargetUser)
{
    if (newTargetUser == mTargetUser)
        return;
    mTargetUser = newTargetUser;
    emit targetUserChanged(mTargetUser);

    auto request = std::make_shared<Helix::UserFollowsRequest>();
    request->fromId = Client::get()->authorization()->userId();
    request->toId = mTargetUser;
    connect(request.get(), &Request::responseReceived,
            [this, request] (const std::shared_ptr<Response> &response)
    {
        auto data = std::unique_ptr<Helix::FollowsList>(static_cast<Helix::FollowsList*>(response->object.release()));
        const bool followed = data->total > 0;
        setFollowed(followed);
    });
    Client::get()->send(request);
}

void FollowManager::follow()
{
    auto request = std::make_shared<v5::FollowChannelRequest>();
    request->userId = Client::get()->authorization()->userId();
    request->channelId = mTargetUser;
    connect(request.get(), &Request::responseReceived,
            [this, request] (const std::shared_ptr<Response> &response)
    {
        Q_UNUSED(response) //TODO: Error checking
        setFollowed(true);
    });
    Client::get()->send(request);
}

void FollowManager::unfollow()
{
    auto request = std::make_shared<v5::FollowChannelRequest>();
    request->userId = Client::get()->authorization()->userId();
    request->channelId = mTargetUser;
    connect(request.get(), &Request::responseReceived,
            [this, request] (const std::shared_ptr<Response> &response)
    {
        Q_UNUSED(response) //TODO: Error checking
        setFollowed(false);
    });
    Client::get()->send(request);
}

void FollowManager::setFollowed(const bool newFollowed)
{
    if (newFollowed != mFollowed) {
        mFollowed = newFollowed;
        emit followedChanged(mFollowed);
    }
}

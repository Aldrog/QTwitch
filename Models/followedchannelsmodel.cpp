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

#include "followedchannelsmodel.h"
#include <Api/client.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

FollowedChannelsModel::FollowedChannelsModel(QObject *parent)
    : HelixScrollableModel(parent)
{
    request = std::make_shared<Helix::UserFollowsRequest>();
    request->fromId = Client::getClient()->getUserId();
    connect(request.get(), &Request::responseReceived, this, &FollowedChannelsModel::receiveFollows);

    usersRequest = std::make_shared<Helix::UsersRequest>();
    connect(usersRequest.get(), &Request::responseReceived, this, &FollowedChannelsModel::receiveStreams);

    streamsRequest = std::make_shared<Helix::StreamsRequest>();
    connect(streamsRequest.get(), &Request::responseReceived, this, &FollowedChannelsModel::receiveStreams);
    reload();
}

void FollowedChannelsModel::receiveFollows(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<Helix::FollowsList>(static_cast<Helix::FollowsList*>(response->object.release()));
    usersRequest->id.clear();
    streamsRequest->userId.clear();
    for (const auto &follow : data->data) {
        usersRequest->id.push_back(follow.toId);
        streamsRequest->userId.push_back(follow.toId);
    }
    auto client = Client::getClient();
    client->send(usersRequest);
    client->send(streamsRequest);
    updateCursor(data->pagination.cursor);
}

void FollowedChannelsModel::receiveUsers(const std::shared_ptr<Response> &response)
{
    usersResponse = response;
    if (streamsResponse)
        pushData();
}

void FollowedChannelsModel::receiveStreams(const std::shared_ptr<Response> &response)
{
    streamsResponse = response;
    if (usersResponse)
        pushData();
}

void FollowedChannelsModel::pushData()
{
    auto users = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(usersResponse->object.release()));
    auto streams = std::unique_ptr<Helix::StreamsList>(static_cast<Helix::StreamsList*>(streamsResponse->object.release()));
    usersResponse.reset();
    streamsResponse.reset();
    for (const auto &user : users->data) {
        const auto &stream = std::find_if(streams->data.begin(), streams->data.end(),
                                          [&user](const Helix::StreamData &s) { return s.userId == user.id; });
        bool live = stream->type == QStringLiteral("live");
        storage.emplace_back( EntitledImage(live ? stream->thumbnailUrl : user.offlineImageUrl, user.displayName),
                              FollowedChannelsModelPayload(live, stream->title, stream->viewerCount) );
    }
}

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

#ifndef FOLLOWEDCHANNELSMODEL_H
#define FOLLOWEDCHANNELSMODEL_H

#include "helixscrollablemodel.h"
#include "payloads.h"
#include <Api/Helix/endpoints.h>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT FollowedChannelsModel : public HelixScrollableModel<FollowedChannelPayload, Api::Helix::UserFollowsRequest>
{
    Q_OBJECT
public:
    explicit FollowedChannelsModel(QObject *parent = nullptr);

    int imageHeight() const override { return imageWidth * 9/16; }

private:
    std::shared_ptr<Api::Helix::UsersRequest> usersRequest;
    std::shared_ptr<Api::Helix::StreamsRequest> streamsRequest;

    std::shared_ptr<QTwitch::Api::Response> usersResponse;
    std::shared_ptr<QTwitch::Api::Response> streamsResponse;

private slots:
    void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response) final;
    void receiveUsers(const std::shared_ptr<QTwitch::Api::Response> &response);
    void receiveStreams(const std::shared_ptr<QTwitch::Api::Response> &response);
    void pushData();
};

}
}

#endif // FOLLOWEDCHANNELSMODEL_H

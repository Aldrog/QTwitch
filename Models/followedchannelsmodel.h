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

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT FollowedChannelsModel : public HelixScrollableModel
{
    Q_OBJECT
public:
    explicit FollowedChannelsModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const final;

    int imageHeight() const override { return imageWidth * 9/16; }

protected:
    inline std::shared_ptr<Api::Helix::PagedRequest> getRequest() const final { return request; }

    inline std::size_t storageSize() const final { return storage.size(); }
    inline void resetStorage() final { storage.clear(); }

private:
    std::shared_ptr<Api::Helix::UserFollowsRequest> request;
    std::shared_ptr<Api::Helix::UsersRequest> usersRequest;
    std::shared_ptr<Api::Helix::StreamsRequest> streamsRequest;

    struct Data {
        Data(EntitledImage img_, FollowedChannelPayload payload_)
            : img(std::move(img_)), payload(std::move(payload_))
        {}
        EntitledImage img;
        FollowedChannelPayload payload;
    };

    std::vector<Data> storage;

    std::shared_ptr<QTwitch::Api::Response> usersResponse;
    std::shared_ptr<QTwitch::Api::Response> streamsResponse;

private slots:
    void receiveFollows(const std::shared_ptr<QTwitch::Api::Response> &response);
    void receiveUsers(const std::shared_ptr<QTwitch::Api::Response> &response);
    void receiveStreams(const std::shared_ptr<QTwitch::Api::Response> &response);
    void pushData();
};

}
}

#endif // FOLLOWEDCHANNELSMODEL_H

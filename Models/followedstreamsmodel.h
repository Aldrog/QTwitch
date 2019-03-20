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

#ifndef FOLLOWEDSTREAMSMODEL_H
#define FOLLOWEDSTREAMSMODEL_H

#include "legacyscrollablemodel.h"
#include "payloads.h"
#include <Api/v5/endpoints.h>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT FollowedStreamsModel : public LegacyScrollableModel<StreamPayload, Api::v5::FollowedStreamsRequest>
{
    Q_OBJECT
public:
    explicit FollowedStreamsModel(QObject *parent = nullptr);

    int imageHeight() const override { return imageWidth * 9/16; }

private slots:
    void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response) final;
};

}
}

#endif // FOLLOWEDSTREAMSMODEL_H

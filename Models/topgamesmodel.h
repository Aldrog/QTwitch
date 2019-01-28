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

#ifndef TOPGAMESMODEL_H
#define TOPGAMESMODEL_H

#include "helixscrollablemodel.h"

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT TopGamesModelPayload
{
    Q_GADGET
    Q_PROPERTY(QString gameId MEMBER gameId)
public:
    TopGamesModelPayload(QString gameId_)
        : gameId(std::move(gameId_))
    {}
    QString gameId;
};

class QTWITCHSHARED_EXPORT TopGamesModel final : public HelixScrollableModel
{
    Q_OBJECT
public:
    explicit TopGamesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const final;

    inline std::size_t storageSize() const final { return storage.size(); }
    inline void resetStorage() final { storage.clear(); }

protected:
    std::shared_ptr<Api::Helix::PagedRequest> getRequest() const final { return request; }

private:
    std::shared_ptr<Api::Helix::TopGamesRequest> request;

    struct Data {
        Data(EntitledImage img_, TopGamesModelPayload payload_)
            : img(std::move(img_)), payload(std::move(payload_))
        {}
        EntitledImage img;
        TopGamesModelPayload payload;
    };

    std::vector<Data> storage;

private slots:
    void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response);
};

}
}

#endif // TOPGAMESMODEL_H

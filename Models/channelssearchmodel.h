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

#ifndef CHANNELSEARCHMODEL_H
#define CHANNELSEARCHMODEL_H

#include "legacyscrollablemodel.h"
#include "payloads.h"

namespace QTwitch {
namespace Models {
namespace Legacy {

class ChannelsSearchModel final : public LegacyScrollableModel
{
    Q_OBJECT
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged RESET resetQuery)
public:
    explicit ChannelsSearchModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const final;

    QString query() const;
    void setQuery(const QString &newQuery);

public slots:
    void resetQuery();

signals:
    void queryChanged(const QString &newQuery);

protected:
    inline std::size_t storageSize() const final { return storage.size(); }
    inline void resetStorage() final { storage.clear(); }

    std::shared_ptr<Api::v5::LegacyPagedRequest> getRequest() const final { return request; }

private:
    std::shared_ptr<Api::v5::SearchChannelsRequest> request;

    struct Data {
        Data(EntitledImage img_, UserPayload payload_)
            : img(std::move(img_)), payload(std::move(payload_))
        {}
        EntitledImage img;
        UserPayload payload;
    };

    std::vector<Data> storage;

private slots:
    void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response);
};

}
}
}

#endif // CHANNELSEARCHMODEL_H

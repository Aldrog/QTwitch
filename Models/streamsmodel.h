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

#ifndef STREAMSMODEL_H
#define STREAMSMODEL_H

#include "helixscrollablemodel.h"

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT StreamsModelPayload
{
    Q_GADGET
    Q_PROPERTY(QString streamTitle MEMBER streamTitle)
    Q_PROPERTY(int viewerCount MEMBER viewerCount)
public:
    StreamsModelPayload(QString title_, int viewers_)
        : streamTitle(std::move(title_)), viewerCount(viewers_)
    {}
    QString streamTitle;
    int viewerCount;
};

class QTWITCHSHARED_EXPORT StreamsModel : public HelixScrollableModel
{
    Q_OBJECT
    Q_PROPERTY(std::vector<QString> gameFilter READ gameFilter WRITE setGameFilter NOTIFY gameFilterChanged RESET resetGameFilter)
    Q_PROPERTY(std::vector<QString> languageFilter READ languageFilter WRITE setLanguageFilter NOTIFY languageFilterChanged RESET resetLanguageFilter)
public:
    explicit StreamsModel(QObject *parent = nullptr);

    std::vector<QString> gameFilter() const;
    std::vector<QString> languageFilter() const;

    void setGameFilter(const std::vector<QString> &newGameFilter);
    void setLanguageFilter(const std::vector<QString> &newLanguageFilter);

    inline std::size_t storageSize() const final { return storage.size(); }
    inline void resetStorage() final { storage.clear(); }

    QVariant data(const QModelIndex &index, int role) const final;

public slots:
    void resetGameFilter();
    void resetLanguageFilter();

signals:
    void gameFilterChanged(const std::vector<QString> &newGameFilter);
    void languageFilterChanged(const std::vector<QString> &newLanguageFilter);

protected:
    std::shared_ptr<Api::Helix::PagedRequest> getRequest() const final { return request; }

private:
    std::shared_ptr<Api::Helix::StreamsRequest> request;

    struct Data {
        Data(EntitledImage img_, StreamsModelPayload payload_)
            : img(std::move(img_)), payload(std::move(payload_))
        {}
        EntitledImage img;
        StreamsModelPayload payload;
    };

    std::vector<Data> storage;

private slots:
    void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response);
};

}
}

#endif // STREAMSMODEL_H

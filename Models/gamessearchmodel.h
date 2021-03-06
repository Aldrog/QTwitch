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

#ifndef GAMESSEARCHMODEL_H
#define GAMESSEARCHMODEL_H

#include "abstractentitledimagesmodel.h"
#include "payloads.h"
#include <Api/v5/endpoints.h>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT GamesSearchModel : public EntitledImagesModel<GamePayload>
{
    Q_OBJECT
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged RESET resetQuery)
public:
    explicit GamesSearchModel(QObject *parent = nullptr);

    inline int pageSize() const override { return 0; }
    inline void setPageSize(int) override {}
    inline void resetPageSize() override {}

    QString query() const;
    void setQuery(const QString &newQuery);

    int imageHeight() const override { return imageWidth * 4/3; }

public slots:
    void resetQuery();

    inline void next() override {}
    inline bool nextAvailable() const override { return false; }
    void reload() override;

signals:
    void queryChanged(const QString &query);

private:
    std::shared_ptr<Api::v5::SearchGamesRequest> request;

protected slots:
    void receiveData(const std::shared_ptr<QTwitch::Api::Response> &response);
};

}
}

#endif // GAMESSEARCHMODEL_H

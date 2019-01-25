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

#ifndef LEGACYSCROLLABLEMODEL_H
#define LEGACYSCROLLABLEMODEL_H

#include "abstractentitledimagesmodel.h"
#include <Api/v5/endpoints.h>

namespace QTwitch {
namespace Models {

class LegacyScrollableModel : public AbstractEntitledImagesModel
{
    Q_OBJECT
public:
    explicit LegacyScrollableModel(QObject *parent = nullptr);

    void next() override;
    bool nextAvailable() const override;
    void reload() override;

protected:
    virtual std::shared_ptr<Api::v5::LegacyPagedRequest> getRequest() const = 0;

    void updateTotal(int total);

private:
    unsigned int totalCount = 0;
};

}
}

#endif // LEGACYSCROLLABLEMODEL_H

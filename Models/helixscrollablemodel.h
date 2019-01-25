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

#ifndef HELIXSCROLLABLEMODEL_H
#define HELIXSCROLLABLEMODEL_H

#include "abstractentitledimagesmodel.h"
#include <Api/Helix/endpoints.h>

namespace QTwitch {
namespace Models {

class HelixScrollableModel : public AbstractEntitledImagesModel
{
    Q_OBJECT
public:
    explicit HelixScrollableModel(QObject *parent = nullptr);

    void next() override;
    bool nextAvailable() const override;
    void reload() override;

protected:
    virtual std::shared_ptr<Api::Helix::PagedRequest> getRequest() const = 0;

    void updateCursor(const QString &cursor);

private:
    QString pagingCursor;
};

}
}

#endif // HELIXSCROLLABLEMODEL_H

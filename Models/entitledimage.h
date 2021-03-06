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

#ifndef ENTITLEDIMAGE_H
#define ENTITLEDIMAGE_H

#include "qtwitch_global.h"
#include <QString>

namespace QTwitch {
namespace Models {

struct QTWITCHSHARED_EXPORT EntitledImage
{
    EntitledImage(QString _img, QString _title)
        : imageUrl(std::move(_img)), title(std::move(_title))
    {}

    QString imageUrl;
    QString title;
};

}
}

#endif // ENTITLEDIMAGE_H

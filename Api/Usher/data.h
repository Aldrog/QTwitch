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

#ifndef USHER_DATA_H
#define USHER_DATA_H

#include "response.h"

namespace QTwitch {
namespace Api {
namespace Usher {

class QTWITCHSHARED_EXPORT AccessToken : public Object
{
public:
    bool mobileRestricted;
    QString sig;
    QString token;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(mobileRestricted, "mobile_restricted");
        visitor.visit(sig, "sig");
        visitor.visit(token, "token");
    }
};

class QTWITCHSHARED_EXPORT PlaylistItem : public Object
{
public:
    QString id;
    QString name;
    QString url;
};

class QTWITCHSHARED_EXPORT Playlist : public Object
{
public:
    std::vector<PlaylistItem> playlist;
};

}
}
}

#endif // USHER_DATA_H

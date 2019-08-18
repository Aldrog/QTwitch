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

#ifndef EXPERIMENTAL_DATA_H
#define EXPERIMENTAL_DATA_H

#include <Api/response.h>
#include <Api/v5/data.h>

namespace QTwitch {
namespace Api {
namespace Experimental {

class QTWITCHSHARED_EXPORT GameData : public Object
{
public:
    int viewers;
    int channels;
    v5::GameData game;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(viewers, "viewers");
        visitor.visit(channels, "channels");
        visitor.visit(game, "game");
    }
};

class QTWITCHSHARED_EXPORT FollowedGamesList : public Object
{
public:
    int total;
    std::vector<GameData> follows;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(total, "_total");
        visitor.visit(follows, "follows");
    }
};

}
}
}

#endif // EXPERIMENTAL_DATA_H

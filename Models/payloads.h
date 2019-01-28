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

#ifndef PAYLOADS_H
#define PAYLOADS_H

#include "qtwitch_global.h"
#include <QString>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT GamePayload
{
    Q_GADGET
    Q_PROPERTY(QString gameId MEMBER gameId)
public:
    explicit GamePayload(QString gameId_)
        : gameId(std::move(gameId_))
    {}
    QString gameId;
};

class QTWITCHSHARED_EXPORT StreamPayload
{
    Q_GADGET
    Q_PROPERTY(QString streamTitle MEMBER streamTitle)
    Q_PROPERTY(int viewerCount MEMBER viewerCount)
public:
    StreamPayload(QString title_, int viewers_)
        : streamTitle(std::move(title_)), viewerCount(viewers_)
    {}
    QString streamTitle;
    int viewerCount;
};

class QTWITCHSHARED_EXPORT UserPayload
{
    Q_GADGET
    Q_PROPERTY(QString userId MEMBER userId)
public:
    explicit UserPayload(QString userId_)
        : userId(std::move(userId_))
    {}
    QString userId;
};

class QTWITCHSHARED_EXPORT FollowedChannelPayload
{
    Q_GADGET
    Q_PROPERTY(QString streamTitle MEMBER streamTitle)
    Q_PROPERTY(int viewerCount MEMBER viewerCount)
public:
    FollowedChannelPayload(bool live_, QString title_, int viewers_)
        : streamTitle(std::move(title_)), viewerCount(viewers_), live(live_)
    {}
    QString streamTitle;
    int viewerCount;
    bool live;
};

}
}

#endif // PAYLOADS_H

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

#ifndef FOLLOWMANAGER_H
#define FOLLOWMANAGER_H

#include <QObject>

namespace QTwitch {
namespace Models {

class FollowManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString targetUser READ targetUser WRITE setTargetUser NOTIFY targetUserChanged)
    Q_PROPERTY(bool followed READ followed NOTIFY followedChanged)
public:
    explicit FollowManager(QObject *parent = nullptr);

    inline bool followed() const { return mFollowed; }
    inline QString targetUser() const { return mTargetUser; }
    void setTargetUser(const QString &newTargetUser);

signals:
    void targetUserChanged(const QString &targetUser);
    void followedChanged(bool followed);

public slots:
    void follow();
    void unfollow();

private:
    bool mFollowed = false;
    QString mTargetUser;

    void setFollowed(bool newFollowed);
};

}
}

#endif // FOLLOWMANAGER_H

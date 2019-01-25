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

#ifndef HELIX_DATA_H
#define HELIX_DATA_H

#include <Api/response.h>

namespace QTwitch {
namespace Api {
namespace Helix {

class QTWITCHSHARED_EXPORT Pagination : public Object
{
public:
    QString cursor;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(cursor, "cursor");
    }
};

class QTWITCHSHARED_EXPORT GameData : public Object
{
public:
    QString id;
    QString name;
    QString boxArtUrl;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "id");
        visitor.visit(name, "name");
        visitor.visit(boxArtUrl, "box_art_url");
    }
};

class QTWITCHSHARED_EXPORT GamesList : public Object
{
public:
    std::vector<GameData> data;
    Pagination pagination;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(data, "data");
        visitor.visit(pagination, "pagination");
    }
};

class QTWITCHSHARED_EXPORT StreamData : public Object
{
public:
    QString id;
    QString userId;
    QString userName;
    QString gameId;
    std::vector<QString> communityIds;
    QString type;
    QString title;
    int viewerCount;
    QString startedAt; /* RFC3339 format. It's not supported by QDateTime ATM. */
    QString language;
    QString thumbnailUrl;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "id");
        visitor.visit(userId, "user_id");
        visitor.visit(userName, "user_name");
        visitor.visit(gameId, "game_id");
        visitor.visit(communityIds, "community_ids");
        visitor.visit(type, "type");
        visitor.visit(title, "title");
        visitor.visit(viewerCount, "viewer_count");
        visitor.visit(startedAt, "started_at");
        visitor.visit(thumbnailUrl, "thumbnail_url");
    }
};

class QTWITCHSHARED_EXPORT StreamsList : public Object
{
public:
    std::vector<StreamData> data;
    Pagination pagination;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(data, "data");
        visitor.visit(pagination, "pagination");
    }
};

class QTWITCHSHARED_EXPORT UserData : public Object
{
public:
    QString id;
    QString login;
    QString displayName;
    enum class Type {
        Staff, Admin, GlobalMod, None
    } type;
    enum class BroadcasterType {
        Partner, Affiliate, None
    } broadcasterType;
    QString description;
    QString profileImageUrl;
    QString offlineImageUrl;
    int viewCount;
    QString email;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "id");
        visitor.visit(login, "login");
        visitor.visit(displayName, "display_name");

        visitor.visit(description, "description");
        visitor.visit(profileImageUrl, "profile_image_url");
        visitor.visit(viewCount, "view_count");
        visitor.visit(email, "email");
    }

private:
    Type typeFromString(const QString &value) const
    {
        if (value == "staff")
            return Type::Staff;
        if (value == "admin")
            return Type::Admin;
        if (value == "global_mod")
            return Type::GlobalMod;
        return Type::None;
    }
    BroadcasterType broadcasterTypeFromString(const QString &value) const
    {
        if (value == "partner")
            return BroadcasterType::Partner;
        if (value == "affiliate")
            return BroadcasterType::Affiliate;
        return BroadcasterType::None;
    }
};

class QTWITCHSHARED_EXPORT UsersList : public Object
{
public:
    std::vector<UserData> data;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(data, "data");
    }
};

class QTWITCHSHARED_EXPORT FollowData : public Object
{
public:
    QString fromId;
    QString fromName;
    QString toId;
    QString toName;
    QString followedAt; /* RFC3339 format. It's not supported by QDateTime ATM. */

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(fromId, "from_id");
        visitor.visit(fromName, "from_name");
        visitor.visit(toId, "to_id");
        visitor.visit(toName, "to_name");
        visitor.visit(followedAt, "followed_at");
    }
};

class QTWITCHSHARED_EXPORT FollowsList : public Object
{
public:
    std::vector<FollowData> data;
    Pagination pagination;
    int total;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(data, "data");
        visitor.visit(pagination, "pagination");
        visitor.visit(total, "total");
    }
};

class QTWITCHSHARED_EXPORT VideoData : public Object
{
public:
    QString id;
    QString userId;
    QString userName;
    QString title;
    QString description;
    QString createdAt; /* RFC3339 format. It's not supported by QDateTime ATM. */
    QString publishedAt; /* RFC3339 format. It's not supported by QDateTime ATM. */
    QString url;
    QString thumbnailUrl;
    enum class Viewable {
        Public, Private, Unknown
    } viewable;
    int viewCount;
    QString language;
    enum class Type {
        Upload, Archive, Highlight, Unknown
    } type;
    QString duration;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "id");
        visitor.visit(userId, "user_id");
        visitor.visit(userName, "user_name");
        visitor.visit(title, "title");
        visitor.visit(description, "description");
        visitor.visit(createdAt, "created_at");
        visitor.visit(publishedAt, "published_at");
        visitor.visit(url, "url");
        visitor.visit(thumbnailUrl, "thumbnail_url");

        QString viewableString;
        visitor.visit(viewableString, "viewable");
        viewable = viewableFromString(viewableString);

        visitor.visit(viewCount, "view_count");
        visitor.visit(language, "language");

        QString typeString;
        visitor.visit(typeString, "type");
        type = typeFromString(typeString);

        visitor.visit(duration, "duration");
    }

private:
    Viewable viewableFromString(const QString &value) const
    {
        if (value == "public")
            return Viewable::Public;
        if (value == "private")
            return Viewable::Private;
        qWarning() << "Unexpected video viewable field" << value;
        return Viewable::Unknown;
    }
    Type typeFromString(const QString &value) const
    {
        if (value == "upload")
            return Type::Upload;
        if (value == "archive")
            return Type::Archive;
        if (value == "highlight")
            return Type::Highlight;
        qWarning() << "Unexpected video type field" << value;
        return Type::Unknown;
    }
};

class QTWITCHSHARED_EXPORT VideosList : public Object
{
public:
    std::vector<VideoData> data;
    Pagination pagination;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(data, "data");
        visitor.visit(pagination, "pagination");
    }
};

}
}
}

#endif // HELIX_DATA_H

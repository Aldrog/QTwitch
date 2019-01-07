#ifndef HELIX_DATA_H
#define HELIX_DATA_H

#include "response.h"

namespace QTwitch {
namespace Api {
namespace Helix {

class QTWITCHSHARED_EXPORT Pagination : public Object
{
public:
    std::string cursor;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(cursor, "cursor");
    }
};

class QTWITCHSHARED_EXPORT GameData : public Object
{
public:
    std::string id;
    std::string name;
    std::string boxArtUrl;

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
    std::string id;
    std::string userId;
    std::string userName;
    std::string gameId;
    std::vector<std::string> communityIds;
    std::string type;
    std::string title;
    int viewerCount;
    std::string startedAt;
    std::string language;
    std::string thumbnailUrl;

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
    std::string id;
    std::string login;
    std::string displayName;
    enum class Type {
        Staff, Admin, GlobalMod, None
    } type;
    enum class BroadcasterType {
        Partner, Affiliate, None
    } broadcasterType;
    std::string description;
    std::string profileImageUrl;
    std::string offlineImageUrl;
    int viewCount;
    std::string email;

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
    Type typeFromString(const std::string &value) const
    {
        if (value == "staff")
            return Type::Staff;
        if (value == "admin")
            return Type::Admin;
        if (value == "global_mod")
            return Type::GlobalMod;
        return Type::None;
    }
    BroadcasterType broadcasterTypeFromString(const std::string &value) const
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
    std::string fromId;
    std::string fromName;
    std::string toId;
    std::string toName;
    std::string followedAt;

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
    std::string id;
    std::string userId;
    std::string name;
    std::string title;
    std::string description;
    std::string createdAt;
    std::string publishedAt;
    std::string url;
    std::string thumbnailUrl;
    enum class Viewable {
        Public, Private
    } viewable;
    int viewCount;
    std::string language;
    enum class Type {
        Upload, Archive, Highlight
    } type;
    std::string duration;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "id");
        visitor.visit(userId, "user_id");
        visitor.visit(name, "name");
        visitor.visit(title, "title");
        visitor.visit(description, "description");
        visitor.visit(createdAt, "created_at");
        visitor.visit(publishedAt, "published_at");
        visitor.visit(url, "url");
        visitor.visit(thumbnailUrl, "thumbnail_url");

        std::string viewableString;
        visitor.visit(viewableString, "viewable");
        viewable = viewableFromString(viewableString);

        visitor.visit(viewCount, "view_count");
        visitor.visit(language, "language");

        std::string typeString;
        visitor.visit(typeString, "type");
        type = typeFromString(typeString);

        visitor.visit(duration, "duration");
    }

private:
    Viewable viewableFromString(const std::string &value) const
    {
        if (value == "public")
            return Viewable::Public;
        if (value == "private")
            return Viewable::Private;
        throw std::runtime_error("Invalid viewable value");
    }
    Type typeFromString(const std::string &value) const
    {
        if (value == "upload")
            return Type::Upload;
        if (value == "archive")
            return Type::Archive;
        if (value == "highlight")
            return Type::Highlight;
        throw std::runtime_error("Invalid type value");
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

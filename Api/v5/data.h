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

#ifndef V5_DATA_H
#define V5_DATA_H

#include <Api/response.h>

namespace QTwitch {
namespace Api {
namespace v5 {

class QTWITCHSHARED_EXPORT ChannelData : public Object
{
public:
    int id;
    QString broadcasterLanguage;
    QString createdAt;
    QString displayName;
    int followers;
    QString game;
    QString language;
    QString logo;
    bool mature;
    QString name;
    bool partner;
    QString profileBanner;
    QString profileBannerBackgroundColor;
    QString status;
    QString updatedAt;
    QString url;
    QString videoBanner;
    int views;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "_id");
        visitor.visit(broadcasterLanguage, "broadcaster_language");
        visitor.visit(createdAt, "created_at");
        visitor.visit(displayName, "display_name");
        visitor.visit(followers, "followers");
        visitor.visit(game, "game");
        visitor.visit(language, "language");
        visitor.visit(logo, "logo");
        visitor.visit(mature, "mature");
        visitor.visit(name, "name");
        visitor.visit(partner, "partner");
        visitor.visit(profileBanner, "profile_banner");
        visitor.visit(profileBannerBackgroundColor, "profile_banner_background_color");
        visitor.visit(status, "status");
        visitor.visit(updatedAt, "updated_at");
        visitor.visit(url, "url");
        visitor.visit(videoBanner, "video_banner");
        visitor.visit(views, "views");
    }
};

class QTWITCHSHARED_EXPORT FollowData : public Object
{
public:
    ChannelData channel;
    QString createdAt;
    bool notifications;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(channel, "channel");
        visitor.visit(createdAt, "created_at");
        visitor.visit(notifications, "notifications");
    }
};

class QTWITCHSHARED_EXPORT ChannelsList : public Object
{
public:
    int total;
    std::vector<ChannelData> channels;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(total, "_total");
        visitor.visit(channels, "channels");
    }
};

class QTWITCHSHARED_EXPORT PictureSet : public Object
{
public:
    QString largeUrl;
    QString mediumUrl;
    QString smallUrl;
    QString templateUrl; // replace {width} and {height} with custom values

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(largeUrl, "large");
        visitor.visit(mediumUrl, "medium");
        visitor.visit(smallUrl, "small");
        visitor.visit(templateUrl, "template");
    }
};

class QTWITCHSHARED_EXPORT GameData : public Object
{
public:
    int id;
    PictureSet box;
    int giantbombId;
    PictureSet logo;
    QString name;
    int popularity;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "_id");
        visitor.visit(box, "box");
        visitor.visit(giantbombId, "giantbomb_id");
        visitor.visit(logo, "logo");
        visitor.visit(name, "name");
        visitor.visit(popularity, "popularity");
    }
};

class QTWITCHSHARED_EXPORT GamesList : public Object
{
public:
    std::vector<GameData> games;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(games, "games");
    }
};

class QTWITCHSHARED_EXPORT StreamData : public Object
{
public:
    int id;
    double averageFps;
    ChannelData channel;
    QString createdAt;
    int delay;
    QString game;
    bool isPlaylist;
    PictureSet preview;
    int videoHeight;
    int viewers;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(id, "_id");
        visitor.visit(averageFps, "average_fps");
        visitor.visit(createdAt, "created_at");
        visitor.visit(delay, "delay");
        visitor.visit(game, "game");
        visitor.visit(isPlaylist, "is_playlist");
        visitor.visit(preview, "preview");
        visitor.visit(videoHeight, "video_height");
        visitor.visit(viewers, "viewers");
    }
};

class QTWITCHSHARED_EXPORT StreamsList : public Object
{
public:
    int total;
    std::vector<StreamData> streams;

    template<class T>
    void accept(const T &visitor)
    {
        visitor.visit(total, "_total");
        visitor.visit(streams, "streams");
    }
};

}
}
}

#endif // V5_DATA_H

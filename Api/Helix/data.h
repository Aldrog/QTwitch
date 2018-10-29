#ifndef HELIX_DATA_H
#define HELIX_DATA_H

#include "response.h"

namespace QTwitch {
namespace Api {
namespace Helix {

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

class QTWITCHSHARED_EXPORT TopGames : public Object
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

}
}
}

#endif // HELIX_DATA_H

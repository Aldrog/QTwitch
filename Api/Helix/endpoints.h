#ifndef HELIX_ENDPOINTS_H
#define HELIX_ENDPOINTS_H

#include "request.h"
#include "data.h"
#include "jsonsetter.h"

namespace QTwitch {
namespace Api {
namespace Helix {

struct Request : public QTwitch::Api::Request
{
    const std::string baseUrl() const final { return "https://api.twitch.tv/helix/"; }
};

struct QTWITCHSHARED_EXPORT TopGamesRequest : public Request
{
    const std::string endpoint() const final { return "games/top"; }
    const std::vector<std::string> options() const final { return { "after", "before", "first" }; }

    std::unique_ptr<Object> createResponseObject(const QJsonObject &object) const final
    {
        typedef TopGames ObjectType;
        auto result = std::make_unique<ObjectType>();
        JsonSetter visitor(object);
        result->accept(visitor);
        return result;
    }
};

}
}
}

#endif // HELIX_ENDPOINTS_H

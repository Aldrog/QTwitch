#include "request.h"

using namespace QTwitch::Api;

std::string Request::getFullUrl(std::initializer_list< Argument > arguments) const
{
    std::string url = baseUrl() + endpoint();
    if (arguments.size() != 0)
        url += "?";
    for (const auto &a : arguments)
        url += a.first + "=" + a.second + "&";
    if (arguments.size() != 0)
        url.resize(url.size() - 1);
    return url;
}

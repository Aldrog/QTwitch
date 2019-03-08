#include "userinfo.h"
#include <Api/client.h>
#include <Api/Helix/endpoints.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

UserInfo::UserInfo(QObject *parent)
    : QObject(parent)
{
}

void UserInfo::setUserId(const QString &newUserId)
{
    if (newUserId == mUserId)
        return;
    mUserId = newUserId;
    emit userIdChanged(mUserId);

    auto request = std::make_shared<Helix::UsersRequest>();
    request->id = {newUserId};
    connect(request.get(), &Request::responseReceived,
            [this, request] (const std::shared_ptr<Response> &response)
    {
        auto data = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(response->object.release()));
        if (!data->data.empty()) {
            mLogin = data->data.front().login;
            emit loginChanged(mLogin);
            mDisplay = data->data.front().displayName;
            emit displayChanged(mDisplay);
        }
    });
    Client::get()->send(request);
}

void UserInfo::setLogin(const QString &newLogin)
{
    if (newLogin == mLogin)
        return;
    mLogin = newLogin;
    emit loginChanged(mLogin);

    auto request = std::make_shared<Helix::UsersRequest>();
    request->login = {newLogin};
    connect(request.get(), &Request::responseReceived,
            [this, request] (const std::shared_ptr<Response> &response)
    {
        auto data = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(response->object.release()));
        if (!data->data.empty()) {
            mUserId = data->data.front().id;
            emit userIdChanged(mUserId);
            mDisplay = data->data.front().displayName;
            emit displayChanged(mDisplay);
        }
    });
    Client::get()->send(request);
}

void UserInfo::setSelf(bool newSelf)
{
    if (newSelf == mSelf)
        return;
    mSelf = newSelf;
    emit selfChanged(mSelf);
    if (!mSelf)
        return;

    auto request = std::make_shared<Helix::UsersRequest>();
    connect(request.get(), &Request::responseReceived,
            [this, request] (const std::shared_ptr<Response> &response)
    {
        auto data = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(response->object.release()));
        if (!data->data.empty()) {
            mUserId = data->data.front().id;
            emit userIdChanged(mUserId);
            mLogin = data->data.front().login;
            emit loginChanged(mLogin);
            mDisplay = data->data.front().displayName;
            emit displayChanged(mDisplay);
        }
    });
    Client::get()->send(request);
}

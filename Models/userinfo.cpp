#include "userinfo.h"
#include <Api/client.h>
#include <Api/Helix/endpoints.h>

using namespace QTwitch::Models;
using namespace QTwitch::Api;

UserInfo::UserInfo(QObject *parent)
    : QObject(parent),
      request(std::make_shared<Helix::UsersRequest>())
{
    connect(request.get(), &Request::responseReceived, this, &UserInfo::receiveData);
}

void UserInfo::setUserId(const QString &newUserId)
{
    if (newUserId == mUserId)
        return;
    mUserId = newUserId;
    emit userIdChanged(mUserId);
    if (mUserId.isEmpty()) {
        request->id.clear();
        return;
    }

    request->id = {mUserId};
    refresh();
}

void UserInfo::setLogin(const QString &newLogin)
{
    if (newLogin == mLogin)
        return;
    mLogin = newLogin;
    emit loginChanged(mLogin);
    if (mLogin.isEmpty()) {
        request->login.clear();
        return;
    }

    request->login = {mLogin};
    refresh();
}

void UserInfo::setSelf(bool newSelf)
{
    if (newSelf == mSelf)
        return;
    mSelf = newSelf;
    emit selfChanged(mSelf);
    if (!mSelf) {
        this->disconnect(Client::get()->authorization());
        return;
    }

    connect(Client::get()->authorization(), &AuthorizationManager::statusChanged,
            this, &UserInfo::refresh);
    refresh();
}

void UserInfo::receiveData(const std::shared_ptr<Response> &response)
{
    auto data = std::unique_ptr<Helix::UsersList>(static_cast<Helix::UsersList*>(response->object.release()));
    if (!data->data.empty()) {
        const auto userId = data->data.front().id;
        if (userId != mUserId) {
            mUserId = userId;
            emit userIdChanged(mUserId);
        }
        const auto login = data->data.front().login;
        if (login != mLogin) {
            mLogin = login;
            emit loginChanged(mLogin);
        }
        const auto display = data->data.front().displayName;
        if (display != mDisplay) {
            mDisplay = display;
            emit displayChanged(mDisplay);
        }
    }
}

void UserInfo::refresh()
{
    const auto client = Client::get();
    if (request->login.empty()
        && request->id.empty()
        && client->authorization()->status() != AuthorizationManager::Status::Authorized) {
        return;
    }
    client->send(request);
}

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

#include <QString>
#include <QtTest>
#include "client.h"
#include "Helix/endpoints.h"
#include "v5/endpoints.h"
#include "Usher/endpoints.h"
#include "qsettingscredentialsstorage.h"

using namespace QTwitch::Api;
using namespace std;

#define QVERIFY_EXCEPTION(statement) \
do {\
    if (!QTest::qVerify(static_cast<bool>(statement), #statement, "", __FILE__, __LINE__))\
        throw std::runtime_error("QVERIFY failed");\
} while (false)

class ApiTest : public QObject
{
    Q_OBJECT

public:
    ApiTest();

private slots:
    void initTestCase();

    void testCaseSetupInfo();
    void testCaseTopGames();
    void testCaseGames();
    void testCaseUser();
    void testCaseUserFollows();
    void testCaseStreams();
    void testCaseVideos();
    void testCaseAuthorization();
    void testCaseStreamPlaylist();

    void cleanupTestCase();

protected:
    unique_ptr<Client> client = make_unique<Client>();
    QString userId;

    std::shared_ptr<Response> sendRequest(const std::shared_ptr<Request> &request)
    {
        QSignalSpy receiveWatcher(request.get(), &Request::responseReceived);
        QSignalSpy errorWatcher(request.get(), &Request::error);
        QSignalSpy sslErrorWatcher(client.get(), &Client::sslErrors);

        client->send(request);

        receiveWatcher.wait();
        if (!errorWatcher.isEmpty())
            qDebug() << "Network error:" << errorWatcher.first();
        if (!sslErrorWatcher.isEmpty())
            qDebug() << "SSL errors:" << sslErrorWatcher.first();

        QVERIFY_EXCEPTION(errorWatcher.isEmpty());
        QVERIFY_EXCEPTION(sslErrorWatcher.isEmpty());
        QVERIFY_EXCEPTION(!receiveWatcher.isEmpty());
        auto response = receiveWatcher.first().first().value<shared_ptr<Response>>();
        QVERIFY_EXCEPTION(response->request == request);
        return response;
    }
};

ApiTest::ApiTest()
{
}

void ApiTest::initTestCase()
{
    qRegisterMetaType<shared_ptr<Response>>();
}

void ApiTest::cleanupTestCase()
{
}

void ApiTest::testCaseSetupInfo()
{
    qDebug() << "Client ID:" << TWITCH_CLIENT_ID;
    qDebug() << "Library version:" << QTWITCH_VERSION;
}

inline QDebug operator<<(QDebug debug, const std::string &str)
{
    return debug << QString::fromStdString(str);
}

void ApiTest::testCaseTopGames()
{
    auto request = std::make_shared<Helix::TopGamesRequest>();
    request->first = 24;

    qDebug() << "Top games url:" << request->getFullUrl();

    auto response = sendRequest(request);
    auto games = dynamic_pointer_cast<Helix::GamesList> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(games);
    qDebug() << "The most popular game is" << games->data[0].name;
}

void ApiTest::testCaseGames()
{
    auto request = std::make_shared<Helix::GamesRequest>();
    request->name = {"Dota 2", "Hollow Knight", "Pillars of Eternity"};

    qDebug() << "Games url:" << request->getFullUrl();

    auto response = sendRequest(request);
    auto games = dynamic_pointer_cast<Helix::GamesList> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(games);
    QVERIFY(games->data.size() == request->name.size() + request->id.size());
    qDebug() << games->data[0].name << "ID is" << games->data[0].id;
}

void ApiTest::testCaseUser()
{
    auto request = std::make_shared<Helix::UsersRequest>();
    request->login = {"aldrog"};

    qDebug() << "Users url:" << request->getFullUrl();

    auto response = sendRequest(request);
    auto users = dynamic_pointer_cast<Helix::UsersList> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(users);
    QVERIFY(users->data.size() == request->id.size() + request->login.size());
    qDebug() << users->data[0].displayName << "ID is" << users->data[0].id;
    userId = users->data[0].id;
}

void ApiTest::testCaseUserFollows()
{
    auto request = std::make_shared<Helix::UserFollowsRequest>();
    request->fromId = userId;
    request->first = 24;

    qDebug() << "User follows url:" << request->getFullUrl();

    auto response = sendRequest(request);
    auto follows = dynamic_pointer_cast<Helix::FollowsList> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(follows);
    qDebug() << "User follows" << follows->data[0].toName << "since" << follows->data[0].followedAt;
}

void ApiTest::testCaseStreams()
{
    auto request = std::make_shared<Helix::StreamsRequest>();
    request->first = 24;

    qDebug() << "Streams url:" << request->getFullUrl();

    auto response = sendRequest(request);
    auto streams = dynamic_pointer_cast<Helix::StreamsList> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(streams);
    qDebug() << "Most popular stream is" << streams->data[0].title << "by" << streams->data[0].userName;
}

void ApiTest::testCaseVideos()
{
    auto request = std::make_shared<Helix::VideosRequest>();
    request->gameId = "68028";
    request->first = 24;
    request->sort = Helix::VideosRequest::Sorting::Trending;

    qDebug() << "Videos url:" << request->getFullUrl();

    auto response = sendRequest(request);
    auto videos = dynamic_pointer_cast<Helix::VideosList> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(videos);
    qDebug() << "Video by" << videos->data[0].userName << "titled" << videos->data[0].title
             << "was published at" << videos->data[0].publishedAt << "and got" << videos->data[0].viewCount << "views.";
}

void ApiTest::testCaseAuthorization()
{
    client->setCredentialsStorage(std::make_unique<QSettingsCredentialsStorage>(QCoreApplication::applicationDirPath() + "/test.ini", QSettings::Format::IniFormat));
    if (client->authorizationStatus == Client::AuthorizationStatus::Authorized)
        QSKIP("Skipping authorization. Remove test.ini to redo authorization test.");
    QSignalSpy authWatcher(client.get(), &Client::authorizationCompleted);
    connect(client.get(), &Client::authorizationError, [] (Client::AuthorizationError e) { qDebug() << "Authorization error:" << e; });
    qInfo() << "Copy the following into a browser:\n" << client->initiateAuthorization({Client::AuthorizationScope::UserFollowsEdit,
                                                                                        Client::AuthorizationScope::ChatEdit,
                                                                                        Client::AuthorizationScope::ChatRead});
    qDebug() << client->authorizationStatus;
    QTextStream input(stdin);
    qInfo() << "After authorization paste redirect url into the terminal:";
    QString inputLine = input.readLine();
    QUrl redirect(inputLine);
    client->updateAuthorization(redirect);
    authWatcher.wait();
    QVERIFY(client->authorizationStatus == Client::AuthorizationStatus::Authorized);
    QVERIFY(!authWatcher.isEmpty());
}

void ApiTest::testCaseStreamPlaylist()
{
    QString channelId;
    {
        auto request = std::make_shared<Helix::StreamsRequest>();
        request->first = 1;

        qDebug() << "Streams url:" << request->getFullUrl();

        auto response = sendRequest(request);
        auto streams = dynamic_pointer_cast<Helix::StreamsList> (
                     shared_ptr<Object>(move(response->object)) );
        QVERIFY(streams);
        channelId = streams->data[0].userId;
    }
    QString channelLogin;
    {
        auto request = std::make_shared<Helix::UsersRequest>();
        request->id = {channelId};

        qDebug() << "Streams url:" << request->getFullUrl();

        auto response = sendRequest(request);
        auto users = dynamic_pointer_cast<Helix::UsersList> (
                     shared_ptr<Object>(move(response->object)) );
        QVERIFY(users);
        channelLogin = users->data[0].login;
    }
    std::shared_ptr<Usher::AccessToken> token;
    {
        auto request = std::make_shared<Usher::StreamAccessTokenRequest>();
        request->channelId = channelLogin;

        qDebug() << "Access token url:" << request->getFullUrl();

        auto response = sendRequest(request);
        token = dynamic_pointer_cast<Usher::AccessToken> (
                     shared_ptr<Object>(move(response->object)) );
        QVERIFY(token);
    }
    {
        auto request = std::make_shared<Usher::StreamPlaylistRequest>();
        request->channelId = channelLogin;
        request->sig = token->sig;
        request->token = token->token;

        qDebug() << "Playlist url:" << request->getFullUrl();

        auto response = sendRequest(request);
        auto playlist = dynamic_pointer_cast<Usher::Playlist> (
                     shared_ptr<Object>(move(response->object)) );
        QVERIFY(playlist);
        for (const auto &i : playlist->playlist)
            qDebug() << i.id << i.name << i.url;
    }


}

QTEST_MAIN(ApiTest)

#include "apitest.moc"

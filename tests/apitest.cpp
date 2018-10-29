#include <QString>
#include <QtTest>
#include "client.h"
#include "Helix/endpoints.h"

using namespace QTwitch::Api;
using namespace std;

class ApiTest : public QObject
{
    Q_OBJECT

public:
    ApiTest();

private slots:
    void initTestCase();

    void testCaseSetupInfo();
    void testCaseTopGames();

    void cleanupTestCase();

protected:
    unique_ptr<Client> client = make_unique<Client>();
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

inline QDebug operator<<(QDebug debug, std::string str)
{
    return debug << QString::fromStdString(str);
}

void ApiTest::testCaseTopGames()
{
    auto request = std::make_shared<Helix::TopGamesRequest>();

    qDebug() << "Top games options:" << request->options();
    qDebug() << "Top games url:" << request->getFullUrl({ {"first", "24"} });

    QSignalSpy receiveWatcher(client.get(), &Client::receive);
    QSignalSpy errorWatcher(client.get(), &Client::error);
    QSignalSpy sslErrorWatcher(client.get(), &Client::sslErrors);

    client->send(request);

    receiveWatcher.wait();
    if (!errorWatcher.isEmpty())
        qDebug() << "Network error:" << errorWatcher.first();
    if (!sslErrorWatcher.isEmpty())
        qDebug() << "SSL errors:" << sslErrorWatcher.first();

    QVERIFY(errorWatcher.isEmpty());
    QVERIFY(sslErrorWatcher.isEmpty());
    QVERIFY(!receiveWatcher.isEmpty());
    auto response = receiveWatcher.first().first().value<shared_ptr<Response>>();
    QVERIFY(response->request == request);
    auto games = dynamic_pointer_cast<Helix::TopGames> (
                 shared_ptr<Object>(move(response->object)) );
    QVERIFY(games);
    qDebug() << "The most popular game is" << games->data[0].name;
}

QTEST_MAIN(ApiTest)

#include "apitest.moc"

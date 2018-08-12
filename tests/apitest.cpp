#include <QString>
#include <QtTest>
#include <client.h>

using namespace QTwitch::Api;
using namespace std;

class ApiTest : public QObject
{
    Q_OBJECT

public:
    ApiTest();

private slots:
    void initTestCase();
    void testCaseEmpty();
    void cleanupTestCase();

protected:
    unique_ptr<Client> client = make_unique<Client>();
};

ApiTest::ApiTest()
{
}

void ApiTest::initTestCase()
{
}

void ApiTest::cleanupTestCase()
{
}

void ApiTest::testCaseEmpty()
{
    qDebug() << "Client ID:" << TWITCH_CLIENT_ID;
    qDebug() << "Library version:" << QTWITCH_VERSION;
}

QTEST_MAIN(ApiTest)

#include "apitest.moc"

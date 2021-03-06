#ifndef USERINFO_H
#define USERINFO_H

#include "qtwitch_global.h"
#include <QObject>
#include <Api/Helix/endpoints.h>

namespace QTwitch {
namespace Models {

class QTWITCHSHARED_EXPORT UserInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userId READ userId WRITE setUserId NOTIFY userIdChanged)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(bool self READ self WRITE setSelf NOTIFY selfChanged)
    Q_PROPERTY(QString display READ display NOTIFY displayChanged)
public:
    explicit UserInfo(QObject *parent = nullptr);

    inline QString userId() const { return mUserId; }
    inline QString login() const { return mLogin; }
    inline bool self() const { return mSelf; }
    inline QString display() const { return mDisplay; }

    void setUserId(const QString &newUserId);
    void setLogin(const QString &newLogin);
    void setSelf(bool newSelf);

signals:
    void userIdChanged(const QString &userId);
    void loginChanged(const QString &login);
    void selfChanged(bool self);
    void displayChanged(const QString &display);

private slots:
    void receiveData(const std::shared_ptr<Api::Response> &response);
    void refresh();

private:
    std::shared_ptr<Api::Helix::UsersRequest> request;

    QString mUserId;
    QString mLogin;
    QString mDisplay;
    bool mSelf = false;
};

}
}

#endif // USERINFO_H

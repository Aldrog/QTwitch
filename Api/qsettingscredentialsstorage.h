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

#ifndef QSETTINGSCREDENTIALSSTORAGE_H
#define QSETTINGSCREDENTIALSSTORAGE_H

#include "qtwitch_global.h"
#include "credentials.h"

#include <QSettings>

namespace QTwitch {
namespace Api {

/*
 * This class stores credentials in plain text.
 * Consider subclassing AbstractCredentialsStorage with OS-specific secrets APIs instead.
 */
class QTWITCHSHARED_EXPORT QSettingsCredentialsStorage : public AbstractCredentialsStorage, public QSettings
{
public:
    using QSettings::QSettings;

    void readCredentials(Credentials &credentials) const final;
    void writeCredentials(const Credentials &credentials) final;
    void clearCredentials() final;

private:
    static const QString authTokenKey;
    static const QString userIdKey;
    static const QString loginKey;
};

}
}

#endif // QSETTINGSCREDENTIALSSTORAGE_H

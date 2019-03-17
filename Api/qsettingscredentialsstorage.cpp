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

#include "qsettingscredentialsstorage.h"

using namespace QTwitch::Api;

void QSettingsCredentialsStorage::readCredentials(Credentials &credentials) const
{
    credentials.authToken = value(authTokenKey).toString();
    credentials.userId    = value(userIdKey   ).toString();
    credentials.login     = value(loginKey    ).toString();
}

void QSettingsCredentialsStorage::writeCredentials(const Credentials &credentials)
{
    setValue(authTokenKey, credentials.authToken);
    setValue(userIdKey   , credentials.userId);
    setValue(loginKey    , credentials.login);
}

void QSettingsCredentialsStorage::clearCredentials()
{
    remove(authTokenKey);
    remove(userIdKey);
    remove(loginKey);
}

const QString QSettingsCredentialsStorage::authTokenKey = QStringLiteral("Credentials/authToken");
const QString QSettingsCredentialsStorage::userIdKey    = QStringLiteral("Credentials/userId");
const QString QSettingsCredentialsStorage::loginKey     = QStringLiteral("Credentials/login");

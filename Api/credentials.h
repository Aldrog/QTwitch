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

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "qtwitch_global.h"

#include <QString>

namespace QTwitch {
namespace Api {

struct QTWITCHSHARED_EXPORT Credentials
{
    QString authToken;
    QString userId;
    QString login;

    void clear()
    {
        authToken.clear();
        userId.clear();
        login.clear();
    }
};

class QTWITCHSHARED_EXPORT AbstractCredentialsStorage
{
public:
    virtual ~AbstractCredentialsStorage() = default;

    virtual void readCredentials(Credentials &credentials) const = 0;
    virtual void writeCredentials(const Credentials &credentials) = 0;
    virtual void clearCredentials() = 0;
};

}
}

#endif // CREDENTIALS_H

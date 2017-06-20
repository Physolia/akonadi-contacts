/*
    This file is part of Contact Editor.

    Copyright (c) 2010 Tobias Koenig <tokoe@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/
#include "imaddress.h"

IMAddress::IMAddress()
    : mProtocol(QStringLiteral("messaging/aim"))
    , mPreferred(false)
{
}

IMAddress::IMAddress(const QString &protocol, const QString &name, bool preferred)
    : mProtocol(protocol)
    , mName(name)
    , mPreferred(preferred)
{
}

void IMAddress::setProtocol(const QString &protocol)
{
    mProtocol = protocol;
}

QString IMAddress::protocol() const
{
    return mProtocol;
}

void IMAddress::setName(const QString &name)
{
    mName = name;
}

QString IMAddress::name() const
{
    return mName;
}

void IMAddress::setPreferred(bool preferred)
{
    mPreferred = preferred;
}

bool IMAddress::preferred() const
{
    return mPreferred;
}

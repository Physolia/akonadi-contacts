/*
    This file is part of Akonadi Contact.

    Copyright (C) 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    Copyright (C) 2016 Laurent Montel <laurent.montel@kdab.com>

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

#ifndef ADDRESSGRANTLEEOBJECT_H
#define ADDRESSGRANTLEEOBJECT_H

#include <QObject>
#include <KContacts/Address>
class AddressGrantleeObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString removeAddressAction READ removeAddressAction)
    Q_PROPERTY(QString modifyAddressAction READ modifyAddressAction)
    Q_PROPERTY(QString formattedAddress READ formattedAddress)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(bool preferredAddress READ preferredAddress)
public:
    explicit AddressGrantleeObject(const KContacts::Address &address, int addressIndex, QObject *parent = Q_NULLPTR);
    ~AddressGrantleeObject();

    QString removeAddressAction() const;
    QString modifyAddressAction() const;
    QString formattedAddress() const;
    QString type() const;
    bool preferredAddress() const;
private:
    QString createActionUrl(const QString &actionName) const;
    KContacts::Address mAddress;
    int mAddressIndex;
};

#endif // ADDRESSGRANTLEEOBJECT_H
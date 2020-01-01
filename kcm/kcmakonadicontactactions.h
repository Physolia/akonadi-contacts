/*
    This file is part of Akonadi Contact.

    Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>
    Copyright (c) 2009 Tobias Koenig <tokoe@kde.org>

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

#ifndef KCMAKONADICONTACTACTIONS_H
#define KCMAKONADICONTACTACTIONS_H

#include <KCModule>
#include "ui_akonadicontactactions.h"

class KConfigDialogManager;

class KCMAkonadiContactActions : public KCModule
{
    Q_OBJECT

public:
    KCMAkonadiContactActions(QWidget *parent, const QVariantList &args);

    void load() override;
    void save() override;
    void defaults() override;

private Q_SLOTS:
    void slotDialPhoneNumberActionChanged(int);
    void slotSmsPhoneNumberActionChanged(int value);
    void slotShowAddressActionChanged(int value);
private:
    KConfigDialogManager *mConfigManager = nullptr;
    Ui_AkonadiContactActions ui;
};

#endif

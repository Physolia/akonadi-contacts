/*
    This file is part of Akonadi Contact.

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

#include "dialphonenumberaction.h"

#include "contactactionssettings.h"
#include "qdialer.h"
#include "qsflphonedialer.h"
#include "qskypedialer.h"
#include "qekigadialer.h"

#include <kcontacts/phonenumber.h>
#include <KLocalizedString>
#include <KMessageBox>
#include <KRun>

#include <QDesktopServices>

using namespace Akonadi;

void DialPhoneNumberAction::dialNumber(const KContacts::PhoneNumber &number)
{
    // synchronize
    ContactActionsSettings::self()->load();

    QDialer *dialer = nullptr;
    // we handle skype separated
    if (ContactActionsSettings::self()->dialPhoneNumberAction() == ContactActionsSettings::UseSkype) {
        dialer = new QSkypeDialer(QStringLiteral("AkonadiContacts"));
    } else if (ContactActionsSettings::self()->dialPhoneNumberAction() == ContactActionsSettings::UseSflPhone) {
        dialer = new QSflPhoneDialer(QStringLiteral("AkonadiContacts"));
    } else if (ContactActionsSettings::self()->dialPhoneNumberAction() == ContactActionsSettings::UseEkiga) {
        dialer = new QEkigaDialer(QStringLiteral("AkonadiContacts"));
    }
    if (dialer) {
        if (!dialer->dialNumber(number.normalizedNumber())) {
            KMessageBox::sorry(nullptr, dialer->errorMessage());
        }
        delete dialer;
        return;
    }

    if (ContactActionsSettings::self()->dialPhoneNumberAction() == ContactActionsSettings::UseSystemDefault) {
        QUrl url;
        url.setScheme(QStringLiteral("tel"));
        url.setPath(number.normalizedNumber());
        QDesktopServices::openUrl(url);
        return;
    }

    QString command = ContactActionsSettings::self()->phoneCommand();
    if (command.isEmpty()) {
        KMessageBox::sorry(nullptr, i18n("There is no application set which could be executed.\nPlease go to the settings dialog and configure one."));
        return;
    }

    /*
     * %N the raw number
     * %n the number with all additional non-number characters removed
     */
    command.replace(QLatin1String("%N"), number.number());
    command.replace(QLatin1String("%n"), number.normalizedNumber());

    KRun::runCommand(command, nullptr);
}

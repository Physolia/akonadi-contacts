/*
    This file is part of Akonadi Contact.

    SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showaddressaction.h"

#include "contactactionssettings.h"

#include <KContacts/Address>
#include <KDialogJobUiDelegate>
#include <KIO/CommandLauncherJob>
#include <ki18n_version.h>
#if KI18N_VERSION >= QT_VERSION_CHECK(5, 88, 0)
#include <KCountry>
#endif

#include <QDesktopServices>

using namespace Akonadi;

static void replaceArguments(QString &templateStr, const KContacts::Address &address)
{
#if KI18N_VERSION >= QT_VERSION_CHECK(5, 88, 0)
    const auto country = KCountry::fromName(address.country()); // will also handle ISO 3166-1 alpha 2 or 3 codes
    templateStr.replace(QLatin1String("%s"), address.street())
        .replace(QLatin1String("%r"), address.region())
        .replace(QLatin1String("%l"), address.locality())
        .replace(QLatin1String("%z"), address.postalCode())
        .replace(QLatin1String("%n"), country.name())
        .replace(QLatin1String("%c"), country.alpha2());
#else
    templateStr.replace(QLatin1String("%s"), address.street())
        .replace(QLatin1String("%r"), address.region())
        .replace(QLatin1String("%l"), address.locality())
        .replace(QLatin1String("%z"), address.postalCode())
        .replace(QLatin1String("%n"), address.country())
        .replace(QLatin1String("%c"), address.countryToISO(address.country()));
#endif
}

void ShowAddressAction::showAddress(const KContacts::Address &address)
{
    // synchronize
    ContactActionsSettings::self()->load();

    if (ContactActionsSettings::self()->showAddressAction() == ContactActionsSettings::UseBrowser) {
        QString urlTemplate = ContactActionsSettings::self()->addressUrl();
        replaceArguments(urlTemplate, address);
        if (!urlTemplate.isEmpty()) {
            QDesktopServices::openUrl(QUrl(urlTemplate));
        }
    } else if (ContactActionsSettings::self()->showAddressAction() == ContactActionsSettings::UseExternalAddressApplication) {
        QString commandTemplate = ContactActionsSettings::self()->addressCommand();
        replaceArguments(commandTemplate, address);

        if (!commandTemplate.isEmpty()) {
            auto job = new KIO::CommandLauncherJob(commandTemplate);
            job->setUiDelegate(new KDialogJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, nullptr));
            job->start();
        }
    } else if (ContactActionsSettings::self()->showAddressAction() == ContactActionsSettings::UseGooglemap) {
        QString urlTemplate = QStringLiteral("https://maps.google.com/maps?q=%s,%l,%c");
        replaceArguments(urlTemplate, address);
        if (!urlTemplate.isEmpty()) {
            QDesktopServices::openUrl(QUrl(urlTemplate));
        }
    } else if (ContactActionsSettings::self()->showAddressAction() == ContactActionsSettings::UseMapquest) {
        QString urlTemplate = QStringLiteral("https://www.mapquest.com/search/result?query=%s,%l,%c");
        replaceArguments(urlTemplate, address);
        if (!urlTemplate.isEmpty()) {
            QDesktopServices::openUrl(QUrl(urlTemplate));
        }
    } else if (ContactActionsSettings::self()->showAddressAction() == ContactActionsSettings::UseOpenStreetMap) {
        QString urlTemplate = QStringLiteral("https://www.openstreetmap.org/search?query=%s,%l,%c");
        replaceArguments(urlTemplate, address);
        if (!urlTemplate.isEmpty()) {
            QDesktopServices::openUrl(QUrl(urlTemplate));
        }
    }
}

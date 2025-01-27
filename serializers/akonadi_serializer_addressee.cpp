/*
    SPDX-FileCopyrightText: 2007 Till Adam <adam@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "akonadi_serializer_addressee.h"

#include <Akonadi/AbstractDifferencesReporter>
#include <Akonadi/Item>

#include "serializer_debug.h"
#include <KContacts/Addressee>
#include <KLocalizedString>
#include <QIODevice>

using namespace Akonadi;

//// ItemSerializerPlugin interface

// FIXME: these values map to Akonadi::ContactParts::Standard and
// Akonadi::ContactParts::Lookup from AkonadiContact library. We don't use those
// symbols here, because AkonadiContact library links to Qt5WebEngine, which when
// loaded tries to initialize OpenGL. When this plugin gets loaded by Akonadi
// from KRunner, it happens in a non-GUI thread, leading to a crash in Qt5WebEngine.
#define CONTACTPART_STANDARD "CONTACT_STANDARD"
#define CONTACTPART_LOOKUP "CONTACT_LOOKUP"

bool SerializerPluginAddressee::deserialize(Item &item, const QByteArray &label, QIODevice &data, int version)
{
    Q_UNUSED(version)

    KContacts::Addressee addr;
    if (label == Item::FullPayload) {
        addr = m_converter.parseVCard(data.readAll());
    } else if (label == CONTACTPART_STANDARD) {
        addr = m_converter.parseVCard(data.readAll());

        // remove pictures and sound
        addr.setPhoto(KContacts::Picture());
        addr.setLogo(KContacts::Picture());
        addr.setSound(KContacts::Sound());
    } else if (label == CONTACTPART_LOOKUP) {
        const KContacts::Addressee temp = m_converter.parseVCard(data.readAll());

        // copy only uid, name and email addresses
        addr.setUid(temp.uid());
        addr.setPrefix(temp.prefix());
        addr.setGivenName(temp.givenName());
        addr.setAdditionalName(temp.additionalName());
        addr.setFamilyName(temp.familyName());
        addr.setSuffix(temp.suffix());
        addr.setEmails(temp.emails());
    } else {
        return false;
    }

    if (!addr.isEmpty()) {
        item.setPayload<KContacts::Addressee>(addr);
    } else {
        qCWarning(AKONADI_SERIALIZER_CONTACT_LOG) << "Empty addressee object!";
    }

    return true;
}

void SerializerPluginAddressee::serialize(const Item &item, const QByteArray &label, QIODevice &data, int &version)
{
    Q_UNUSED(version)

    if (label != Item::FullPayload && label != CONTACTPART_STANDARD && label != CONTACTPART_LOOKUP) {
        return;
    }

    if (!item.hasPayload<KContacts::Addressee>()) {
        return;
    }

    KContacts::Addressee addr;
    KContacts::Addressee temp;

    temp = item.payload<KContacts::Addressee>();

    if (label == Item::FullPayload) {
        addr = temp;
    } else if (label == CONTACTPART_STANDARD) {
        addr = temp;

        // remove pictures and sound
        addr.setPhoto(KContacts::Picture());
        addr.setLogo(KContacts::Picture());
        addr.setSound(KContacts::Sound());
    } else if (label == CONTACTPART_LOOKUP) {
        // copy only uid, name and email addresses
        addr.setUid(temp.uid());
        addr.setPrefix(temp.prefix());
        addr.setGivenName(temp.givenName());
        addr.setAdditionalName(temp.additionalName());
        addr.setFamilyName(temp.familyName());
        addr.setSuffix(temp.suffix());
        addr.setEmails(temp.emails());
    }

    data.write(m_converter.createVCard(addr));
}

//// DifferencesAlgorithmInterface interface

static bool compareString(const QString &left, const QString &right)
{
    if (left.isEmpty() && right.isEmpty()) {
        return true;
    } else {
        return left == right;
    }
}

static QString toString(const KContacts::PhoneNumber &phoneNumber)
{
    return phoneNumber.number();
}

static QString toString(const KContacts::Address &address)
{
    return address.toString();
}

static QString toString(const QString &value)
{
    return value;
}

template<class T>
static void compareList(Akonadi::AbstractDifferencesReporter *reporter, const QString &id, const QList<T> &left, const QList<T> &right)
{
    for (int i = 0; i < left.count(); ++i) {
        if (!right.contains(left[i])) {
            reporter->addProperty(AbstractDifferencesReporter::AdditionalLeftMode, id, toString(left[i]), QString());
        }
    }

    for (int i = 0; i < right.count(); ++i) {
        if (!left.contains(right[i])) {
            reporter->addProperty(AbstractDifferencesReporter::AdditionalRightMode, id, QString(), toString(right[i]));
        }
    }
}

template<class T>
static void compareVector(Akonadi::AbstractDifferencesReporter *reporter, const QString &id, const QList<T> &left, const QList<T> &right)
{
    for (int i = 0; i < left.count(); ++i) {
        if (!right.contains(left[i])) {
            reporter->addProperty(AbstractDifferencesReporter::AdditionalLeftMode, id, toString(left[i]), QString());
        }
    }

    for (int i = 0; i < right.count(); ++i) {
        if (!left.contains(right[i])) {
            reporter->addProperty(AbstractDifferencesReporter::AdditionalRightMode, id, QString(), toString(right[i]));
        }
    }
}

void SerializerPluginAddressee::compare(Akonadi::AbstractDifferencesReporter *reporter, const Akonadi::Item &leftItem, const Akonadi::Item &rightItem)
{
    Q_ASSERT(reporter);
    Q_ASSERT(leftItem.hasPayload<KContacts::Addressee>());
    Q_ASSERT(rightItem.hasPayload<KContacts::Addressee>());

    reporter->setLeftPropertyValueTitle(i18n("Changed Contact"));
    reporter->setRightPropertyValueTitle(i18n("Conflicting Contact"));

    const auto leftContact = leftItem.payload<KContacts::Addressee>();
    const auto rightContact = rightItem.payload<KContacts::Addressee>();

    if (!compareString(leftContact.uid(), rightContact.uid())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::uidLabel(), leftContact.uid(), rightContact.uid());
    }

    if (!compareString(leftContact.name(), rightContact.name())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::nameLabel(), leftContact.name(), rightContact.name());
    }

    if (!compareString(leftContact.formattedName(), rightContact.formattedName())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::formattedNameLabel(),
                              leftContact.formattedName(),
                              rightContact.formattedName());
    }

    if (!compareString(leftContact.familyName(), rightContact.familyName())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::familyNameLabel(),
                              leftContact.familyName(),
                              rightContact.familyName());
    }

    if (!compareString(leftContact.givenName(), rightContact.givenName())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::givenNameLabel(),
                              leftContact.givenName(),
                              rightContact.givenName());
    }

    if (!compareString(leftContact.additionalName(), rightContact.additionalName())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::additionalNameLabel(),
                              leftContact.additionalName(),
                              rightContact.additionalName());
    }

    if (!compareString(leftContact.prefix(), rightContact.prefix())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::prefixLabel(), leftContact.prefix(), rightContact.prefix());
    }

    if (!compareString(leftContact.suffix(), rightContact.suffix())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::suffixLabel(), leftContact.suffix(), rightContact.suffix());
    }

    if (!compareString(leftContact.nickName(), rightContact.nickName())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::nickNameLabel(),
                              leftContact.nickName(),
                              rightContact.nickName());
    }

    if (leftContact.birthday() != rightContact.birthday()) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::birthdayLabel(),
                              leftContact.birthday().toString(),
                              rightContact.birthday().toString());
    }

    if (!compareString(leftContact.mailer(), rightContact.mailer())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::mailerLabel(), leftContact.mailer(), rightContact.mailer());
    }

    if (leftContact.timeZone() != rightContact.timeZone()) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::timeZoneLabel(),
                              leftContact.timeZone().toString(),
                              rightContact.timeZone().toString());
    }

    if (leftContact.geo() != rightContact.geo()) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::geoLabel(),
                              leftContact.geo().toString(),
                              rightContact.geo().toString());
    }

    if (!compareString(leftContact.title(), rightContact.title())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::titleLabel(), leftContact.title(), rightContact.title());
    }

    if (!compareString(leftContact.role(), rightContact.role())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::roleLabel(), leftContact.role(), rightContact.role());
    }

    if (!compareString(leftContact.organization(), rightContact.organization())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::organizationLabel(),
                              leftContact.organization(),
                              rightContact.organization());
    }

    if (!compareString(leftContact.note(), rightContact.note())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode, KContacts::Addressee::noteLabel(), leftContact.note(), rightContact.note());
    }

    if (!compareString(leftContact.productId(), rightContact.productId())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::productIdLabel(),
                              leftContact.productId(),
                              rightContact.productId());
    }

    if (!compareString(leftContact.sortString(), rightContact.sortString())) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::sortStringLabel(),
                              leftContact.sortString(),
                              rightContact.sortString());
    }

    if (leftContact.secrecy() != rightContact.secrecy()) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::secrecyLabel(),
                              leftContact.secrecy().toString(),
                              rightContact.secrecy().toString());
    }

    if (leftContact.url() != rightContact.url()) {
        reporter->addProperty(AbstractDifferencesReporter::ConflictMode,
                              KContacts::Addressee::urlLabel(),
                              leftContact.url().url().toDisplayString(),
                              rightContact.url().url().toDisplayString());
    }

    compareList(reporter, i18n("Emails"), leftContact.emails(), rightContact.emails());
    compareVector(reporter, i18n("Phone Numbers"), leftContact.phoneNumbers(), rightContact.phoneNumbers());
    compareVector(reporter, i18n("Addresses"), leftContact.addresses(), rightContact.addresses());

    // TODO: logo/photo/custom entries
}

//// GidExtractorInterface

QString SerializerPluginAddressee::extractGid(const Item &item) const
{
    if (!item.hasPayload<KContacts::Addressee>()) {
        return {};
    }
    return item.payload<KContacts::Addressee>().uid();
}

#include "moc_akonadi_serializer_addressee.cpp"

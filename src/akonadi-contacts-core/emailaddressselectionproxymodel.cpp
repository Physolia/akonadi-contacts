/*
    This file is part of Akonadi Contact.

    SPDX-FileCopyrightText: 2010 KDAB
    SPDX-FileContributor: Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emailaddressselectionproxymodel_p.h"

#include <Akonadi/Item>
#include <KContacts/Addressee>
#include <KContacts/ContactGroup>
#include <KLocalizedString>

using namespace Akonadi;

static QString createToolTip(const KContacts::ContactGroup &group)
{
    QString txt = QStringLiteral("<qt>");

    txt += QStringLiteral("<b>%1</b>").arg(i18n("Distribution List %1", group.name()));
    txt += QLatin1String("<ul>");
    const uint groupDataCount(group.dataCount());
    for (uint i = 0; i < groupDataCount; ++i) {
        txt += QLatin1String("<li>");
        txt += group.data(i).name() + QLatin1Char(' ');
        txt += QLatin1String("<em>");
        txt += group.data(i).email();
        txt += QLatin1String("</em></li>");
    }
    txt += QLatin1String("</ul>");
    txt += QLatin1String("</qt>");

    return txt;
}

static QString createToolTip(const QString &name, const QString &email)
{
    return QStringLiteral("<qt>%1<b>%2</b></qt>").arg(name.isEmpty() ? QString() : name + QLatin1String("<br/>"), email);
}

EmailAddressSelectionProxyModel::EmailAddressSelectionProxyModel(QObject *parent)
    : LeafExtensionProxyModel(parent)
{
}

EmailAddressSelectionProxyModel::~EmailAddressSelectionProxyModel() = default;

QVariant EmailAddressSelectionProxyModel::data(const QModelIndex &index, int role) const
{
    const QVariant value = LeafExtensionProxyModel::data(index, role);

    if (!value.isValid()) { // index is not a leaf child
        if (role == NameRole) {
            const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
            if (item.hasPayload<KContacts::Addressee>()) {
                const auto contact = item.payload<KContacts::Addressee>();
                return contact.realName();
            } else if (item.hasPayload<KContacts::ContactGroup>()) {
                const auto group = item.payload<KContacts::ContactGroup>();
                return group.name();
            }
        } else if (role == EmailAddressRole) {
            const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
            if (item.hasPayload<KContacts::Addressee>()) {
                const auto contact = item.payload<KContacts::Addressee>();
                return contact.preferredEmail();
            } else if (item.hasPayload<KContacts::ContactGroup>()) {
                const auto group = item.payload<KContacts::ContactGroup>();
                return group.name(); // the name must be resolved by the caller
            }
        } else if (role == Qt::ToolTipRole) {
            const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
            if (item.hasPayload<KContacts::Addressee>()) {
                const auto contact = item.payload<KContacts::Addressee>();
                return createToolTip(contact.realName(), contact.preferredEmail());
            } else if (item.hasPayload<KContacts::ContactGroup>()) {
                return createToolTip(item.payload<KContacts::ContactGroup>());
            }
        }
    }

    return value;
}

int EmailAddressSelectionProxyModel::leafRowCount(const QModelIndex &index) const
{
    const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
    if (item.hasPayload<KContacts::Addressee>()) {
        const auto contact = item.payload<KContacts::Addressee>();
        if (contact.emails().count() == 1) {
            return 0;
        } else {
            return contact.emails().count();
        }
    } else if (item.hasPayload<KContacts::ContactGroup>()) {
        const auto group = item.payload<KContacts::ContactGroup>();
        return group.dataCount();
    } else {
        return 0;
    }
}

int EmailAddressSelectionProxyModel::leafColumnCount(const QModelIndex &index) const
{
    const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
    if (item.hasPayload<KContacts::Addressee>()) {
        return 1;
    } else if (item.hasPayload<KContacts::ContactGroup>()) {
        return 1;
    } else {
        return 0;
    }
}

QVariant EmailAddressSelectionProxyModel::leafData(const QModelIndex &index, int row, int, int role) const
{
    if (role == Qt::DisplayRole) {
        const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
        if (item.hasPayload<KContacts::Addressee>()) {
            const auto contact = item.payload<KContacts::Addressee>();
            if (row >= 0 && row < contact.emails().count()) {
                return contact.emails().at(row);
            }
        } else if (item.hasPayload<KContacts::ContactGroup>()) {
            const auto group = item.payload<KContacts::ContactGroup>();
            if (row >= 0 && row < (int)group.dataCount()) {
                return i18nc("Name and email address of a contact", "%1 <%2>", group.data(row).name(), group.data(row).email());
            }
        }
    } else if (role == NameRole) {
        const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
        if (item.hasPayload<KContacts::Addressee>()) {
            const auto contact = item.payload<KContacts::Addressee>();
            return contact.realName();
        } else if (item.hasPayload<KContacts::ContactGroup>()) {
            const auto group = item.payload<KContacts::ContactGroup>();
            if (row >= 0 && row < (int)group.dataCount()) {
                return group.data(row).name();
            }
        }
    } else if (role == EmailAddressRole) {
        const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
        if (item.hasPayload<KContacts::Addressee>()) {
            const auto contact = item.payload<KContacts::Addressee>();
            if (row >= 0 && row < contact.emails().count()) {
                return contact.emails().at(row);
            }
        } else if (item.hasPayload<KContacts::ContactGroup>()) {
            const auto group = item.payload<KContacts::ContactGroup>();
            if (row >= 0 && row < (int)group.dataCount()) {
                return group.data(row).email();
            }
        }
    } else if (role == Qt::ToolTipRole) {
        const auto item = index.data(ContactsTreeModel::ItemRole).value<Akonadi::Item>();
        if (item.hasPayload<KContacts::Addressee>()) {
            const auto contact = item.payload<KContacts::Addressee>();
            if (row >= 0 && row < contact.emails().count()) {
                return createToolTip(contact.realName(), contact.emails().at(row));
            }
        } else if (item.hasPayload<KContacts::ContactGroup>()) {
            const auto group = item.payload<KContacts::ContactGroup>();
            if (row >= 0 && row < (int)group.dataCount()) {
                return createToolTip(group.data(row).name(), group.data(row).email());
            }
        }
    } else {
        return index.data(role);
    }

    return {};
}

#include "moc_emailaddressselectionproxymodel_p.cpp"

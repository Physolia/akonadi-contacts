/*
   SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef ABSTRACTEMAILADDRESSSELECTIONDIALOG_H
#define ABSTRACTEMAILADDRESSSELECTIONDIALOG_H

#include "akonadi-contact_export.h"

#include "emailaddressselectionwidget.h"

#include <QDialog>
#include <KContacts/Addressee>
namespace Akonadi {
class AKONADI_CONTACT_EXPORT AbstractEmailAddressSelectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AbstractEmailAddressSelectionDialog(QWidget *parent = nullptr);
    virtual ~AbstractEmailAddressSelectionDialog();

    /**
     * Returns the list of selected email addresses.
     */
    Q_REQUIRED_RESULT virtual EmailAddressSelection::List selectedAddresses() const = 0;

    /**
     * Returns the email address selection view that is used.
     */
    Q_REQUIRED_RESULT virtual EmailAddressSelectionWidget *view() const = 0;
Q_SIGNALS:
    void insertAddresses(const KContacts::Addressee::List &list);
};
}

#endif // ABSTRACTEMAILADDRESSSELECTIONDIALOG_H

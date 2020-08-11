/*
    This file is part of Contact Editor.

    SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef ADDRESSTYPEDIALOG_H
#define ADDRESSTYPEDIALOG_H

#include <QDialog>
#include <KContacts/Address>

class QButtonGroup;

namespace ContactEditor {
/**
 * Dialog for creating a new address types.
 *
 * @note This dialog is only used by AddressTypeCombo.
 */
class AddressTypeDialog : public QDialog
{
    Q_OBJECT
public:
    AddressTypeDialog(KContacts::Address::Type type, QWidget *parent);
    ~AddressTypeDialog();

    KContacts::Address::Type type() const;

private:
    QButtonGroup *mGroup = nullptr;

    KContacts::Address::TypeList mTypeList;
};
}

#endif // ADDRESSTYPEDIALOG_H

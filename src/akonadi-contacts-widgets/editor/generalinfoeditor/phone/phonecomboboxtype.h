/*
    This file is part of Contact Editor.

    SPDX-FileCopyrightText: 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi_contact_widgets_private_export.h"
#include <KContacts/PhoneNumber>
#include <editor/widgets/contacteditorcombobox.h>
namespace Akonadi
{
/**
 * @short A combobox to select a phone number type.
 */
class AKONADI_CONTACT_WIDGETS_TESTS_EXPORT PhoneComboBoxType : public Akonadi::ContactEditorComboBox
{
    Q_OBJECT

public:
    /**
     * Creates a phone type combo.
     *
     * @param parent The parent widget.
     */
    explicit PhoneComboBoxType(QWidget *parent = nullptr);

    /**
     * Destroys the phone type combo.
     */
    ~PhoneComboBoxType() override;

    /**
     * Sets the phone number @p type that shall be selected.
     */
    void setType(KContacts::PhoneNumber::Type type);

    /**
     * Returns the selected phone number type.
     */
    KContacts::PhoneNumber::Type type() const;

    void resetToDefault();

private:
    void selected(int);
    void otherSelected();
    void update();
    KContacts::PhoneNumber::Type mType = KContacts::PhoneNumber::Home;
    int mLastSelected = 0;
    QList<int> mTypeList;
};
}

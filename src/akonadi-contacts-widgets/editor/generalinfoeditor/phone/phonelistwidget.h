/*
    This file is part of Contact Editor.

    SPDX-FileCopyrightText: 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi_contact_widgets_private_export.h"
#include <QWidget>
namespace KContacts
{
class Addressee;
}
namespace Akonadi
{
class PhoneWidgetLister;
class AKONADI_CONTACT_WIDGETS_TESTS_EXPORT PhoneListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhoneListWidget(QWidget *parent = nullptr);
    ~PhoneListWidget() override;
    void loadContact(const KContacts::Addressee &contact);
    void storeContact(KContacts::Addressee &contact) const;
    void setReadOnly(bool readOnly);

private:
    Akonadi::PhoneWidgetLister *mPhoneWidgetLister = nullptr;
};
}

/*
    SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "contacteditor_export.h"
#include <Akonadi/CollectionDialog>

#include <memory>

namespace AkonadiContactWidgets
{
class SelectAddressBookDialogPrivate;
/**
 * @brief The SelectAddressBookDialog class This class allows to select addressbook where saving contacts
 * @since 5.1
 */
class CONTACTEDITOR_EXPORT SelectAddressBookDialog : public Akonadi::CollectionDialog
{
    Q_OBJECT
public:
    explicit SelectAddressBookDialog(QWidget *parent = nullptr);
    ~SelectAddressBookDialog() override;

private:
    std::unique_ptr<SelectAddressBookDialogPrivate> const d;
};
}

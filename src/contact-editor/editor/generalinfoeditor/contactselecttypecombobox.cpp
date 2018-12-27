/*
    This file is part of Contact Editor.

    Copyright (C) 2016-2019 Laurent Montel <montel@kde.org>

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

#include "contactselecttypecombobox.h"
#include <KLocalizedString>

using namespace ContactEditor;

ContactSelectTypeCombobox::ContactSelectTypeCombobox(QWidget *parent)
    : ContactEditor::ContactEditorComboBox(parent)
{
    initialize();
}

ContactSelectTypeCombobox::~ContactSelectTypeCombobox()
{
}

void ContactSelectTypeCombobox::initialize()
{
    addItem(i18n("Select..."), QString());
    QString type = QStringLiteral("HOME");
    mSelectType.append(type);
    addItem(i18n("Home"), type);
    type = QStringLiteral("WORK");
    mSelectType.append(type);
    addItem(i18n("Work"), type);
    type = QStringLiteral("OTHER");
    mSelectType.append(type);
    addItem(i18n("Other"), type);
}

QStringList ContactSelectTypeCombobox::selectTypeList() const
{
    return mSelectType;
}

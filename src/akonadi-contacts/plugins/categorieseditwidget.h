/*
    This file is part of Contact Editor.

    Copyright (c) 2010 Tobias Koenig <tokoe@kde.org>

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

#ifndef CATEGORIESEDITWIDGET_H
#define CATEGORIESEDITWIDGET_H

#include <QWidget>

#include <kcontacts/addressee.h>
#include <ContactEditor/CategoriesEditAbstractWidget>
#include <QVariant>
namespace Akonadi {
class TagWidget;
}

/**
 * @short A widget for editing the categories of a contact.
 */
class CategoriesEditWidget : public ContactEditor::CategoriesEditAbstractWidget
{
    Q_OBJECT

public:
    explicit CategoriesEditWidget(QWidget *parent = nullptr, const QList<QVariant> & =
    {
    });
    ~CategoriesEditWidget();

    void loadContact(const KContacts::Addressee &contact) override;
    void storeContact(KContacts::Addressee &contact) const override;

    void setReadOnly(bool readOnly) override;

private:
    Akonadi::TagWidget *mTagWidget;
};

#endif
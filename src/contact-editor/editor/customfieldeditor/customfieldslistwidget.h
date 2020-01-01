/*
    This file is part of Contact Editor.

    Copyright (C) 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    Copyright (C) 2016-2020 Laurent Montel <montel.org>

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

#ifndef CUSTOMFIELDSLISTWIDGET_H
#define CUSTOMFIELDSLISTWIDGET_H

#include <QWidget>
#include "customfieldsmodel.h"
class QTreeView;
namespace KContacts {
class Addressee;
}
class CustomFieldsModel;
namespace ContactEditor {
class CustomFieldsListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomFieldsListWidget(QWidget *parent = nullptr);
    ~CustomFieldsListWidget();
    void storeContact(KContacts::Addressee &contact) const;
    void loadContact(const KContacts::Addressee &contact);
    void setReadOnly(bool readOnly);

    void setLocalCustomFieldDescriptions(const QVariantList &descriptions);
    Q_REQUIRED_RESULT QVariantList localCustomFieldDescriptions() const;
public Q_SLOTS:
    void slotAddNewField(const CustomField &field);

private:
    CustomField::List mLocalCustomFields;
    QTreeView *mCustomFieldList = nullptr;
    CustomFieldsModel *mModel = nullptr;
};
}
#endif // CUSTOMFIELDSLISTWIDGET_H

/*
    This file is part of Akonadi Contact.

    Copyright (C) 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    Copyright (C) 2016-2017 Laurent Montel <laurent.montel@kdab.com>

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

#ifndef NAMEWIDGET_H
#define NAMEWIDGET_H

#include "displaynameeditwidget.h"
#include <QWidget>

#include <KContacts/Addressee>
class KLineEdit;
class QToolButton;
namespace Akonadi
{
class NameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NameWidget(QWidget *parent = nullptr);
    ~NameWidget();

    void loadContact(const KContacts::Addressee &contact);
    void storeContact(KContacts::Addressee &contact) const;
    void setReadOnly(bool readOnly);

    void setDisplayType(DisplayNameEditWidget::DisplayType type);
    DisplayNameEditWidget::DisplayType displayType() const;

Q_SIGNALS:
    void nameChanged(const KContacts::Addressee &contact);

private:
    void slotTextChanged(const QString &text);
    void slotOpenNameEditDialog();
    DisplayNameEditWidget::DisplayType mDisplayType;
    KContacts::Addressee mContact;
    KLineEdit *mNameEdit;
    QToolButton *mButtonEdit;
};
}
#endif // NAMEWIDGET_H

/*
   Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef RECIPIENTSPICKERWIDGET_H
#define RECIPIENTSPICKERWIDGET_H

#include <QWidget>
#include "akonadi-contact_export.h"
class QAbstractItemModel;
class QTreeView;
namespace Akonadi {
class EmailAddressSelectionWidget;
class AKONADI_CONTACT_EXPORT RecipientsPickerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RecipientsPickerWidget(bool onlyShowEmailWithAddress, QAbstractItemModel *model, QWidget *parent = nullptr);
    ~RecipientsPickerWidget();
    Q_REQUIRED_RESULT QTreeView *view() const;

    Q_REQUIRED_RESULT Akonadi::EmailAddressSelectionWidget *emailAddressSelectionWidget() const;
private:
    Akonadi::EmailAddressSelectionWidget *mView = nullptr;
};
}

#endif // RECIPIENTSPICKERWIDGET_H
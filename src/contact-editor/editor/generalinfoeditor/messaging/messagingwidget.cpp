/*
    This file is part of Contact Editor.

    Copyright (C) 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    Copyright (C) 2016-2019 Laurent Montel <laurent.montel@kdab.com>

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

#include "messagingwidget.h"
#include "../../widgets/contacteditorcombobox.h"
#include <editor/widgets/preferredlineeditwidget.h>

#include <KContacts/Impp>

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QToolButton>
#include <QUrl>

using namespace ContactEditor;
MessagingWidget::MessagingWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    mMessagingEdit = new PreferredLineEditWidget(this);
    mMessagingEdit->setTrapReturnKey(true);
    mMessagingEdit->setPlaceholderText(i18n("Add an identifier"));
    mMessagingEdit->setObjectName(QStringLiteral("messaginglineedit"));
    connect(mMessagingEdit, &PreferredLineEditWidget::preferredChanged, this, &MessagingWidget::slotPreferredChanged);
    layout->addWidget(mMessagingEdit);

    mProtocolCombo = new ContactEditor::ContactEditorComboBox(this);
    mProtocolCombo->setObjectName(QStringLiteral("protocol"));
    mProtocolCombo->addItem(i18nc("@item:inlistbox select from a list of IM protocols",
                                  "Select..."));
    layout->addWidget(mProtocolCombo);

    const auto protocols = KContacts::Impp::serviceTypes();
    for (const QString &protocol : protocols) {
        mProtocolCombo->addItem(QIcon::fromTheme(KContacts::Impp::serviceIcon(protocol)),
                                KContacts::Impp::serviceLabel(protocol),
                                protocol);
    }

    mAddButton = new QToolButton(this);
    mAddButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAddButton->setObjectName(QStringLiteral("addbutton"));
    mAddButton->setToolTip(i18n("Add an IM"));
    connect(mAddButton, &QToolButton::clicked, this, &MessagingWidget::slotAddMessaging);
    layout->addWidget(mAddButton);

    mRemoveButton = new QToolButton(this);
    mRemoveButton->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemoveButton->setObjectName(QStringLiteral("removebutton"));
    mRemoveButton->setToolTip(i18n("Remove IM"));
    connect(mRemoveButton, &QToolButton::clicked, this, &MessagingWidget::slotRemoveMessaging);
    layout->addWidget(mRemoveButton);
}

MessagingWidget::~MessagingWidget()
{
}

void MessagingWidget::slotPreferredChanged()
{
    Q_EMIT preferredChanged(this);
}

void MessagingWidget::setIMAddress(const KContacts::Impp &address)
{
    const int idx = mProtocolCombo->findData(address.serviceType());
    if (idx >= 0) {
        mProtocolCombo->setCurrentIndex(idx);
    }
    mMessagingEdit->setText(address.address().path());
    mMessagingEdit->setPreferred(address.isPreferred());
}

void MessagingWidget::setPreferred(bool b)
{
    mMessagingEdit->setPreferred(b);
}

KContacts::Impp MessagingWidget::imAddress() const
{
    QUrl uri;
    uri.setScheme(mProtocolCombo->currentData().toString());
    uri.setPath(mMessagingEdit->text().trimmed());
    KContacts::Impp addr;
    addr.setAddress(uri);
    addr.setPreferred(mMessagingEdit->preferred());
    return addr;
}

void MessagingWidget::updateAddRemoveButton(bool addButtonEnabled)
{
    mAddButton->setEnabled(addButtonEnabled);
}

void MessagingWidget::slotAddMessaging()
{
    Q_EMIT addWidget(this);
}

void MessagingWidget::slotRemoveMessaging()
{
    Q_EMIT removeWidget(this);
}

void MessagingWidget::clearWidget()
{
    mProtocolCombo->setCurrentIndex(0);
    mMessagingEdit->clear();
}

void MessagingWidget::setReadOnly(bool readOnly)
{
    mMessagingEdit->setReadOnly(readOnly);
    mAddButton->setEnabled(!readOnly);
    mRemoveButton->setEnabled(!readOnly);
    mProtocolCombo->setEnabled(!readOnly);
}

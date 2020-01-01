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

#include "webwidget.h"
#include "webselecttypecombobox.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QUrl>
#include <KLocalizedString>
#include <editor/widgets/preferredlineeditwidget.h>
using namespace ContactEditor;

WebWidget::WebWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    mWebSiteEdit = new PreferredLineEditWidget(this);
    mWebSiteEdit->setTrapReturnKey(true);
    mWebSiteEdit->setPlaceholderText(i18n("Add a Web Site"));
    mWebSiteEdit->setObjectName(QStringLiteral("website"));
    layout->addWidget(mWebSiteEdit);
    connect(mWebSiteEdit, &PreferredLineEditWidget::preferredChanged, this, &WebWidget::slotPreferredChanged);

    mWebType = new ContactEditor::WebSelectTypeCombobox(this);
    mWebType->setObjectName(QStringLiteral("webtype"));
    layout->addWidget(mWebType);

    mAddButton = new QToolButton(this);
    mAddButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    mAddButton->setObjectName(QStringLiteral("addbutton"));
    mAddButton->setToolTip(i18n("Add a Web Site"));
    connect(mAddButton, &QToolButton::clicked, this, &WebWidget::slotAddWeb);
    layout->addWidget(mAddButton);

    mRemoveButton = new QToolButton(this);
    mRemoveButton->setObjectName(QStringLiteral("removebutton"));
    mRemoveButton->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    mRemoveButton->setToolTip(i18n("Remove Web Site"));
    connect(mRemoveButton, &QToolButton::clicked, this, &WebWidget::slotRemoveWeb);
    layout->addWidget(mRemoveButton);
}

WebWidget::~WebWidget()
{
}

void WebWidget::clearWidget()
{
    mWebSiteEdit->clear();
    mWebType->setCurrentIndex(0);
}

void WebWidget::updateAddRemoveButton(bool addButtonEnabled)
{
    mAddButton->setEnabled(addButtonEnabled);
}

void WebWidget::slotAddWeb()
{
    Q_EMIT addWidget(this);
}

void WebWidget::slotRemoveWeb()
{
    Q_EMIT removeWidget(this);
}

void WebWidget::setPreferred(bool b)
{
    mWebSiteEdit->setPreferred(b);
}

KContacts::ResourceLocatorUrl WebWidget::url()
{
    mUrl.setUrl(QUrl(mWebSiteEdit->text()));
    mUrl.setPreferred(mWebSiteEdit->preferred());
    mUrl.setType(KContacts::ResourceLocatorUrl::Type(mWebType->currentData().toInt()));
    return mUrl;
}

void WebWidget::loadWebSite(const KContacts::ResourceLocatorUrl &url)
{
    mUrl = url;
    setPreferred(url.isPreferred());
    mWebType->setCurrentIndex(mWebType->findData((int)url.type()));
    mWebSiteEdit->setText(mUrl.url().toDisplayString());
}

void WebWidget::setReadOnly(bool readOnly)
{
    mWebSiteEdit->setReadOnly(readOnly);
    mAddButton->setEnabled(!readOnly);
    mRemoveButton->setEnabled(!readOnly);
    mWebType->setEnabled(!readOnly);
}

void WebWidget::slotPreferredChanged()
{
    Q_EMIT preferredChanged(this);
}

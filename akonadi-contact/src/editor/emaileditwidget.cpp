/*
    This file is part of Akonadi Contact.

    Copyright (c) 2009 Tobias Koenig <tokoe@kde.org>

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

#include "emaileditwidget.h"

#include "autoqpointer_p.h"

#include <QtCore/QEvent>
#include <QtCore/QString>
#include <QGridLayout>
#include <QPushButton>
#include <QToolButton>

#include <kcontacts/addressee.h>
#include <kacceleratormanager.h>
#include <qinputdialog.h>
#include <KLineEdit>
#include <QListWidget>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <KConfigGroup>
#include <KEmailAddress>

class EmailAddressExtracter : public QObject
{
public:
    EmailAddressExtracter(QLineEdit *lineEdit)
        : QObject(lineEdit)
        , mLineEdit(lineEdit)
    {
        lineEdit->installEventFilter(this);
    }

    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE
    {
        if (watched == mLineEdit && event->type() == QEvent::FocusOut) {
            const QString fullEmailAddress = mLineEdit->text();
            const QString extractedEmailAddress = KEmailAddress::extractEmailAddress(fullEmailAddress);
            mLineEdit->setText(extractedEmailAddress);
        }

        return QObject::eventFilter(watched, event);
    }

private:
    QLineEdit *mLineEdit;
};

class EmailItem : public QListWidgetItem
{
public:
    EmailItem(const QString &text, QListWidget *parent, bool preferred)
        : QListWidgetItem(text, parent)
        , mPreferred(preferred)
    {
        format();
    }

    void setEmail(const KContacts::Email &email)
    {
        mEmail = email;
    }

    KContacts::Email email() const
    {
        return mEmail;
    }

    void setPreferred(bool preferred)
    {
        mPreferred = preferred;
        format();
    }
    bool preferred() const
    {
        return mPreferred;
    }

private:
    void format()
    {
        QFont f = font();
        f.setBold(mPreferred);
        setFont(f);
    }

private:
    KContacts::Email mEmail;
    bool mPreferred;
};

EmailEditWidget::EmailEditWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);

    mEmailEdit = new KLineEdit;
    mEmailEdit->setObjectName(QStringLiteral("emailedit"));
    mEmailEdit->setTrapReturnKey(true);
    new EmailAddressExtracter(mEmailEdit);
    connect(mEmailEdit, &QLineEdit::textChanged, this, &EmailEditWidget::textChanged);
    layout->addWidget(mEmailEdit);

    mEditButton = new QToolButton;
    mEditButton->setText(QStringLiteral("..."));
    connect(mEditButton, &QPushButton::clicked, this, &EmailEditWidget::edit);
    mEditButton->setObjectName(QStringLiteral("editbutton"));
    layout->addWidget(mEditButton);
    setFocusProxy(mEditButton);
    setFocusPolicy(Qt::StrongFocus);
}

EmailEditWidget::~EmailEditWidget()
{
}

void EmailEditWidget::setReadOnly(bool readOnly)
{
    mEmailEdit->setReadOnly(readOnly);
    mEditButton->setEnabled(!readOnly);
}

void EmailEditWidget::loadContact(const KContacts::Addressee &contact)
{
    mList = contact.emailList();

    if (mList.isEmpty()) {
        mEmailEdit->setText(QString());
    } else {
        mEmailEdit->setText(mList.first().mail());
    }
}

void EmailEditWidget::storeContact(KContacts::Addressee &contact) const
{
    contact.setEmailList(mList);
}

void EmailEditWidget::edit()
{
    AutoQPointer<EmailEditDialog> dlg = new EmailEditDialog(this);
    dlg->setEmailList(mList);
    if (dlg->exec()) {
        if (dlg->changed()) {
            mList = dlg->emailList();
            if (!mList.isEmpty()) {
                mEmailEdit->setText(mList.first().mail());
            } else {
                mEmailEdit->setText(QString());
            }
        }
    }
}

void EmailEditWidget::textChanged(const QString &text)
{
    if (mList.isEmpty()) {
        mList.append(KContacts::Email(text));
    } else {
        mList.first().setEmail(text);
    }
}

EmailEditDialog::EmailEditDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(i18n("Edit Email Addresses"));

    QWidget *page = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(page);

    QGridLayout *topLayout = new QGridLayout;
    mainLayout->addLayout(topLayout);
    topLayout->setMargin(0);

    mEmailListBox = new QListWidget(page);
    mEmailListBox->setObjectName(QStringLiteral("emailListBox"));
    mainLayout->addWidget(mEmailListBox);
    mEmailListBox->setSelectionMode(QAbstractItemView::SingleSelection);

    // Make sure there is room for the scrollbar
    mEmailListBox->setMinimumHeight(mEmailListBox->sizeHint().height() + 30);
    connect(mEmailListBox, &QListWidget::currentItemChanged, this, &EmailEditDialog::selectionChanged);
    connect(mEmailListBox, &QListWidget::itemDoubleClicked, this, &EmailEditDialog::edit);
    topLayout->addWidget(mEmailListBox, 0, 0, 5, 2);

    mAddButton = new QPushButton(i18n("Add..."), page);
    mAddButton->setObjectName(QStringLiteral("add"));
    mainLayout->addWidget(mAddButton);
    connect(mAddButton, &QPushButton::clicked, this, &EmailEditDialog::add);
    topLayout->addWidget(mAddButton, 0, 2);

    mEditButton = new QPushButton(i18n("Edit..."), page);
    mainLayout->addWidget(mEditButton);
    mEditButton->setObjectName(QStringLiteral("edit"));
    mEditButton->setEnabled(false);
    connect(mEditButton, &QPushButton::clicked, this, &EmailEditDialog::edit);
    topLayout->addWidget(mEditButton, 1, 2);

    mRemoveButton = new QPushButton(i18n("Remove"), page);
    mainLayout->addWidget(mRemoveButton);
    mRemoveButton->setObjectName(QStringLiteral("remove"));
    mRemoveButton->setEnabled(false);
    connect(mRemoveButton, &QPushButton::clicked, this, &EmailEditDialog::remove);
    topLayout->addWidget(mRemoveButton, 2, 2);

    mStandardButton = new QPushButton(i18n("Set as Standard"), page);
    mainLayout->addWidget(mStandardButton);
    mStandardButton->setObjectName(QStringLiteral("standard"));
    mStandardButton->setEnabled(false);
    connect(mStandardButton, &QPushButton::clicked, this, &EmailEditDialog::standard);
    topLayout->addWidget(mStandardButton, 3, 2);

    topLayout->setRowStretch(4, 1);

    // set default state
    KAcceleratorManager::manage(this);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Cancel)->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &EmailEditDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &EmailEditDialog::reject);
    mainLayout->addWidget(buttonBox);

    readConfig();
}

EmailEditDialog::~EmailEditDialog()
{
    writeConfig();
}

void EmailEditDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "EmailEditDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 200));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void EmailEditDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), "EmailEditDialog");
    group.writeEntry("Size", size());
}

KContacts::Email::List EmailEditDialog::emailList() const
{
    KContacts::Email::List lst;

    for (int i = 0; i < mEmailListBox->count(); ++i) {
        EmailItem *item = static_cast<EmailItem *>(mEmailListBox->item(i));
        KContacts::Email email = item->email();
        email.setEmail(item->text());
        if (item->preferred()) {
            lst.prepend(email);
        } else {
            lst.append(email);
        }
    }

    return lst;

}


void EmailEditDialog::add()
{
    bool ok = false;

    QString email = QInputDialog::getText(this, i18n("Add Email"), i18n("New Email:"), QLineEdit::Normal,
                                          QString(), &ok);

    if (!ok || email.trimmed().isEmpty()) {
        return;
    }

    QString errorMessage;
    email = KEmailAddress::extractEmailAddress(email.toLower(), errorMessage);
    if (email.trimmed().isEmpty()) {
        KMessageBox::error(this, errorMessage, i18n("Add Email"));
        return;
    }

    // check if item already available, ignore if so...
    for (int i = 0; i < mEmailListBox->count(); ++i) {
        if (mEmailListBox->item(i)->text() == email) {
            return;
        }
    }

    new EmailItem(email, mEmailListBox, (mEmailListBox->count() == 0));

    mChanged = true;
}

void EmailEditDialog::edit()
{
    bool ok = false;

    QListWidgetItem *item = mEmailListBox->currentItem();

    QString email = QInputDialog::getText(this, i18n("Edit Email"),
                                          i18nc("@label:textbox Inputfield for an email address", "Email:"), QLineEdit::Normal,
                                          item->text(), &ok);

    if (!ok) {
        return;
    }

    QString messageError;
    email = KEmailAddress::extractEmailAddress(email.toLower(), messageError);
    if (email.trimmed().isEmpty()) {
        KMessageBox::error(this, messageError, i18n("Edit Email"));
        return;
    }

    // check if item already available, ignore if so...
    for (int i = 0; i < mEmailListBox->count(); ++i) {
        if (mEmailListBox->item(i)->text() == email) {
            return;
        }
    }

    EmailItem *eitem = static_cast<EmailItem *>(item);
    eitem->setText(email);

    mChanged = true;
}

void EmailEditDialog::remove()
{
    const QString address = mEmailListBox->currentItem()->text();

    const QString text = i18n("<qt>Are you sure that you want to remove the email address <b>%1</b>?</qt>", address);
    const QString caption = i18n("Confirm Remove");

    if (KMessageBox::warningContinueCancel(this, text, caption, KGuiItem(i18n("&Delete"), QStringLiteral("edit-delete"))) == KMessageBox::Continue) {
        EmailItem *item = static_cast<EmailItem *>(mEmailListBox->currentItem());

        const bool preferred = item->preferred();
        mEmailListBox->takeItem(mEmailListBox->currentRow());
        if (preferred) {
            item = dynamic_cast<EmailItem *>(mEmailListBox->item(0));
            if (item) {
                item->setPreferred(true);
            }
        }

        mChanged = true;
    }
}

bool EmailEditDialog::changed() const
{
    return mChanged;
}

void EmailEditDialog::setEmailList(const KContacts::Email::List &list)
{
    QStringList emails;
    bool preferred = true;
    Q_FOREACH(const KContacts::Email &email, list) {
        if (!emails.contains(email.mail())) {
            EmailItem *emailItem = new EmailItem(email.mail(), mEmailListBox, preferred);
            emailItem->setEmail(email);
            emails << email.mail();
            preferred = false;
        }
    }
}

void EmailEditDialog::standard()
{
    for (int i = 0; i < mEmailListBox->count(); ++i) {
        EmailItem *item = static_cast<EmailItem *>(mEmailListBox->item(i));
        if (i == mEmailListBox->currentRow()) {
            item->setPreferred(true);
        } else {
            item->setPreferred(false);
        }
    }

    mChanged = true;
}

void EmailEditDialog::selectionChanged()
{
    const int index = mEmailListBox->currentRow();
    const bool value = (index >= 0);   // An item is selected

    mRemoveButton->setEnabled(value);
    mEditButton->setEnabled(value);
    mStandardButton->setEnabled(value);
}

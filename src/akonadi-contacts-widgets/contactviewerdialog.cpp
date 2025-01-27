/*
    This file is part of Akonadi Contact.

    SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "contactviewerdialog.h"
#include "contactviewer.h"

#include <Akonadi/Item>
using namespace Akonadi;

#include <KConfig>
#include <KLocalizedString>

#include <KConfigGroup>
#include <QDesktopServices>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myContactViewerDialogGroupName[] = "ContactViewer";
}
class Akonadi::ContactViewerDialogPrivate
{
public:
    explicit ContactViewerDialogPrivate(ContactViewerDialog *parent)
        : q(parent)
    {
    }

    void readConfig()
    {
        KConfig config(QStringLiteral("akonadi_contactrc"));
        KConfigGroup group(&config, QLatin1String(myContactViewerDialogGroupName));
        const QSize size = group.readEntry("Size", QSize(500, 600));
        if (size.isValid()) {
            q->resize(size);
        }
    }

    void writeConfig()
    {
        KConfig config(QStringLiteral("akonadi_contactrc"));
        KConfigGroup group(&config, QLatin1String(myContactViewerDialogGroupName));
        group.writeEntry("Size", q->size());
        group.sync();
    }

    ContactViewerDialog *const q;
    Akonadi::ContactViewer *mViewer = nullptr;
};

using namespace Akonadi;

ContactViewerDialog::ContactViewerDialog(QWidget *parent)
    : QDialog(parent)
    , d(new ContactViewerDialogPrivate(this))
{
    setWindowTitle(i18nc("@title:window", "Show Contact"));
    auto mainLayout = new QVBoxLayout(this);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    auto mainWidget = new QWidget(this);
    mainLayout->addWidget(mainWidget);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ContactViewerDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ContactViewerDialog::reject);

    auto layout = new QVBoxLayout(mainWidget);

    d->mViewer = new Akonadi::ContactViewer(this);
    layout->addWidget(d->mViewer);

    connect(d->mViewer, &Akonadi::ContactViewer::urlClicked, this, [](const QUrl &url) {
        QDesktopServices::openUrl(url);
    });

    mainLayout->addWidget(buttonBox);

    d->readConfig();
}

ContactViewerDialog::~ContactViewerDialog()
{
    d->writeConfig();
}

Akonadi::Item ContactViewerDialog::contact() const
{
    return d->mViewer->contact();
}

Akonadi::ContactViewer *ContactViewerDialog::viewer() const
{
    return d->mViewer;
}

void ContactViewerDialog::setContact(const Akonadi::Item &contact)
{
    d->mViewer->setContact(contact);
}

#include "moc_contactviewerdialog.cpp"

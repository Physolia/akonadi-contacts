/*
    This file is part of Akonadi Contact.

    SPDX-FileCopyrightText: 2009 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "contactviewer.h"

#include "attributes/contactmetadataattribute_p.h"
#include "contactmetadataakonadi_p.h"
#include "customfieldmanager_p.h"
#include "standardcontactformatter.h"
#include "textbrowser_p.h"

#include <Akonadi/Collection>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/Item>
#include <Akonadi/ItemFetchScope>
#include <KConfigGroup>
#include <KContacts/Addressee>
#include <KLocalizedString>
#include <kio/transferjob.h>

#include <KConfig>
#include <KContacts/VCardConverter>
#include <Prison/Barcode>
#include <QGuiApplication>
#include <QIcon>
#include <QScreen>
#include <QUrlQuery>
#include <QVBoxLayout>

using namespace Akonadi;
using namespace Akonadi;

class Akonadi::ContactViewerPrivate
{
public:
    explicit ContactViewerPrivate(ContactViewer *parent)
        : mParent(parent)
        , mQRCode(Prison::Barcode::create(Prison::QRCode))
    {
        mStandardContactFormatter = new StandardContactFormatter;
        mContactFormatter = mStandardContactFormatter;
        KConfig config(QStringLiteral("akonadi_contactrc"));
        KConfigGroup group(&config, QStringLiteral("View"));
        mShowQRCode = group.readEntry("QRCodes", true);
    }

    ~ContactViewerPrivate()
    {
        delete mStandardContactFormatter;
    }

    void updateView(const QVariantList &localCustomFieldDescriptions = QVariantList(), const QString &addressBookName = QString())
    {
        static QPixmap defaultPixmap = QIcon::fromTheme(QStringLiteral("user-identity")).pixmap(QSize(100, 100));
        static QPixmap defaultMapPixmap = QIcon::fromTheme(QStringLiteral("map-symbolic")).pixmap(QSize(16, 16));
        static QPixmap defaultSmsPixmap = QIcon::fromTheme(QStringLiteral("message-new")).pixmap(QSize(16, 16));

        mParent->setWindowTitle(i18nc("@title:window", "Contact %1", mCurrentContact.assembledName()));

        if (mCurrentContact.photo().isIntern()) {
            mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_photo")), mCurrentContact.photo().data());
        } else if (!mCurrentContact.photo().url().isEmpty()) {
            QByteArray imageData;
            QImage image;
            KIO::TransferJob *job = KIO::get(QUrl(mCurrentContact.photo().url()), KIO::NoReload);
            QObject::connect(job, &KIO::TransferJob::data, job, [&imageData](KIO::Job *, const QByteArray &data) {
                imageData.append(data);
            });
            if (job->exec()) {
                if (image.loadFromData(imageData)) {
                    mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_photo")), image);
                } else {
                    mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_photo")), defaultPixmap);
                }
            } else {
                mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_photo")), defaultPixmap);
            }
        } else {
            mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_photo")), defaultPixmap);
        }

        if (mCurrentContact.logo().isIntern()) {
            mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_logo")), mCurrentContact.logo().data());
        } else if (!mCurrentContact.logo().url().isEmpty()) {
            QByteArray imageData;
            QImage image;
            KIO::TransferJob *job = KIO::get(QUrl(mCurrentContact.logo().url()), KIO::NoReload);
            QObject::connect(job, &KIO::TransferJob::data, job, [&imageData](KIO::Job *, const QByteArray &data) {
                imageData.append(data);
            });
            if (job->exec()) {
                if (image.loadFromData(imageData)) {
                    mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("contact_logo")), image);
                }
            }
        }

        mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("map_icon")), defaultMapPixmap);

        mBrowser->document()->addResource(QTextDocument::ImageResource, QUrl(QStringLiteral("sms_icon")), defaultSmsPixmap);

        if (mShowQRCode) {
            KContacts::VCardConverter converter;
            KContacts::Addressee addr(mCurrentContact);
            addr.setPhoto(KContacts::Picture());
            addr.setLogo(KContacts::Picture());
            const QString data = QString::fromUtf8(converter.createVCard(addr));
            if (mQRCode) {
                mQRCode->setData(data);
                mBrowser->document()->addResource(QTextDocument::ImageResource,
                                                  QUrl(QStringLiteral("qrcode")),
                                                  mQRCode->toImage(mQRCode->preferredSize(QGuiApplication::primaryScreen()->devicePixelRatio()).toSize()));
            }
        }

        // merge local and global custom field descriptions
        QList<QVariantMap> customFieldDescriptions;
        const CustomField::List globalCustomFields = CustomFieldManager::globalCustomFieldDescriptions();
        customFieldDescriptions.reserve(localCustomFieldDescriptions.count() + globalCustomFields.count());
        for (const QVariant &entry : std::as_const(localCustomFieldDescriptions)) {
            customFieldDescriptions << entry.toMap();
        }

        for (const CustomField &field : std::as_const(globalCustomFields)) {
            QVariantMap description;
            description.insert(QStringLiteral("key"), field.key());
            description.insert(QStringLiteral("title"), field.title());

            customFieldDescriptions << description;
        }

        KContacts::Addressee contact(mCurrentContact);
        if (!addressBookName.isEmpty()) {
            contact.insertCustom(QStringLiteral("KADDRESSBOOK"), QStringLiteral("AddressBook"), addressBookName);
        }

        mContactFormatter->setContact(contact);
        mContactFormatter->setCustomFieldDescriptions(customFieldDescriptions);

        mBrowser->setHtml(mContactFormatter->toHtml());
    }

    void slotUrlClicked(const QUrl &url)
    {
        const QUrlQuery query(url);
        const QString urlScheme(url.scheme());
        if (urlScheme == QLatin1String("http") || urlScheme == QLatin1String("https") || urlScheme == QLatin1String("tel") || urlScheme == QLatin1String("sms")
            || urlScheme == QLatin1String("geo") || urlScheme == QLatin1String("mailto")) {
            Q_EMIT mParent->urlClicked(url);
        } else if (urlScheme == QLatin1String("address")) {
            const int pos = query.queryItemValue(QStringLiteral("index")).toInt();

            const KContacts::Address::List addresses = mCurrentContact.addresses();
            if (pos < addresses.count()) {
                Q_EMIT mParent->addressClicked(addresses.at(pos));
            }
        }
    }

    void slotParentCollectionFetched(KJob *job)
    {
        mParentCollectionFetchJob = nullptr;

        QString addressBookName;

        if (!job->error()) {
            auto fetchJob = qobject_cast<CollectionFetchJob *>(job);
            if (!fetchJob->collections().isEmpty()) {
                const Collection collection = fetchJob->collections().at(0);
                addressBookName = collection.displayName();
            }
        }

        // load the local meta data of the item
        Akonadi::ContactMetaDataAkonadi metaData;
        metaData.load(mCurrentItem);

        updateView(metaData.customFieldDescriptions(), addressBookName);
    }

    QMetaObject::Connection mCollectionFetchJobConnection;
    KContacts::Addressee mCurrentContact;
    Item mCurrentItem;
    ContactViewer *const mParent;
    TextBrowser *mBrowser = nullptr;
    AbstractContactFormatter *mContactFormatter = nullptr;
    AbstractContactFormatter *mStandardContactFormatter = nullptr;
    CollectionFetchJob *mParentCollectionFetchJob = nullptr;
    std::optional<Prison::Barcode> mQRCode;
    bool mShowQRCode = true;
};

ContactViewer::ContactViewer(QWidget *parent)
    : QWidget(parent)
    , d(new ContactViewerPrivate(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins({});

    d->mBrowser = new TextBrowser;

    connect(d->mBrowser, &TextBrowser::anchorClicked, this, [this](const QUrl &url) {
        d->slotUrlClicked(url);
    });

    layout->addWidget(d->mBrowser);

    // always fetch full payload for contacts
    fetchScope().fetchFullPayload();
    fetchScope().fetchAttribute<ContactMetaDataAttribute>();
    fetchScope().setAncestorRetrieval(ItemFetchScope::Parent);
}

ContactViewer::~ContactViewer() = default;

Akonadi::Item ContactViewer::contact() const
{
    return ItemMonitor::item();
}

KContacts::Addressee ContactViewer::rawContact() const
{
    return d->mCurrentContact;
}

void ContactViewer::setContactFormatter(AbstractContactFormatter *formatter)
{
    if (formatter == nullptr) {
        d->mContactFormatter = d->mStandardContactFormatter;
    } else {
        d->mContactFormatter = formatter;
        delete d->mStandardContactFormatter;
        d->mStandardContactFormatter = nullptr;
    }
}

void ContactViewer::setContact(const Akonadi::Item &contact)
{
    ItemMonitor::setItem(contact);
}

void ContactViewer::setRawContact(const KContacts::Addressee &contact)
{
    d->mCurrentContact = contact;

    d->updateView();
}

void ContactViewer::itemChanged(const Item &contactItem)
{
    if (!contactItem.hasPayload<KContacts::Addressee>()) {
        return;
    }

    d->mCurrentItem = contactItem;
    d->mCurrentContact = contactItem.payload<KContacts::Addressee>();

    // stop any running fetch job
    if (d->mParentCollectionFetchJob) {
        disconnect(d->mCollectionFetchJobConnection);
        delete d->mParentCollectionFetchJob;
        d->mParentCollectionFetchJob = nullptr;
    }

    d->mParentCollectionFetchJob = new CollectionFetchJob(contactItem.parentCollection(), CollectionFetchJob::Base, this);
    d->mCollectionFetchJobConnection = connect(d->mParentCollectionFetchJob, &CollectionFetchJob::result, this, [this](KJob *job) {
        d->slotParentCollectionFetched(job);
    });
}

void ContactViewer::itemRemoved()
{
    d->mBrowser->clear();
}

void ContactViewer::updateView()
{
    itemChanged(d->mCurrentItem);
}

void ContactViewer::setShowQRCode(bool b)
{
    if (d->mShowQRCode != b) {
        d->mShowQRCode = b;
        updateView();
    }
}

bool ContactViewer::showQRCode() const
{
    return d->mShowQRCode;
}

#include "moc_contactviewer.cpp"

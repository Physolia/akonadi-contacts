/*
  Copyright 2013-2020 Laurent Montel <montel@kde.org>

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

#ifndef ADDEMAILDISPLAYJOB_H
#define ADDEMAILDISPLAYJOB_H

#include "akonadi-contact_export.h"
#include <AkonadiCore/Item>
#include <KJob>

namespace Akonadi {
class Item;
/**
 * @brief The AddEmailDisplayJob class
 * @author Laurent Montel <montel@kde.org>
 */
class AKONADI_CONTACT_EXPORT AddEmailDisplayJob : public KJob
{
    Q_OBJECT

public:
    explicit AddEmailDisplayJob(const QString &email, QWidget *parentWidget, QObject *parent = nullptr);

    ~AddEmailDisplayJob() override;
    void setShowAsHTML(bool html);
    void setRemoteContent(bool b);
    void setContact(const Akonadi::Item &contact);
    void setMessageId(Akonadi::Item::Id id);

    void start() override;

Q_SIGNALS:
    void contactUpdated(const Akonadi::Item &contact, Akonadi::Item::Id id, bool showAsHTML, bool remoteContent);

private:
    //@cond PRIVATE
    class Private;
    Private *const d;
    //@endcond
};
}

#endif
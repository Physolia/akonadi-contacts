/*  This file is part of the KDE project
    SPDX-FileCopyrightText: 2017-2023 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "akonadi-contact-core_export.h"

/* Classes which are exported only for unit tests */
#ifdef BUILD_TESTING
#ifndef AKONADI_CONTACTS_TESTS_EXPORT
#define AKONADI_CONTACTS_TESTS_EXPORT AKONADI_CONTACT_CORE_EXPORT
#endif
#else /* not compiling tests */
#define AKONADI_CONTACTS_TESTS_EXPORT
#endif
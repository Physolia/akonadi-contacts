/*
    This file is part of Contact Editor.

    SPDX-FileCopyrightText: 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    SPDX-FileCopyrightText: 2016-2020 Laurent Montel <montel.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "addresslocationwidgettest.h"
#include "../addresslocationwidget.h"
#include <QLabel>
#include <QPushButton>
#include <KComboBox>
#include <KLineEdit>
#include <QTest>
#include <QCheckBox>
#include "../selectaddresstypecombobox.h"

using namespace ContactEditor;

AddressLocationWidgetTest::AddressLocationWidgetTest(QObject *parent)
    : QObject(parent)
{
}

AddressLocationWidgetTest::~AddressLocationWidgetTest()
{
}

void AddressLocationWidgetTest::shouldHaveDefaultValue()
{
    AddressLocationWidget w;

    SelectAddressTypeComboBox *typeAddress = w.findChild<SelectAddressTypeComboBox *>(QStringLiteral("typeaddress"));
    QVERIFY(typeAddress);

    QLabel *streetlabel = w.findChild<QLabel *>(QStringLiteral("streetlabel"));
    QVERIFY(streetlabel);
    KLineEdit *mStreetEdit = w.findChild<KLineEdit *>(QStringLiteral("streetlineedit"));
    QVERIFY(mStreetEdit);

    QLabel *postofficeboxlabel = w.findChild<QLabel *>(QStringLiteral("postofficeboxlabel"));
    QVERIFY(postofficeboxlabel);
    KLineEdit *postofficeboxlineedit = w.findChild<KLineEdit *>(QStringLiteral("postofficeboxlineedit"));
    QVERIFY(postofficeboxlineedit);

    QLabel *localitylabel = w.findChild<QLabel *>(QStringLiteral("localitylabel"));
    QVERIFY(localitylabel);
    KLineEdit *localitylineedit = w.findChild<KLineEdit *>(QStringLiteral("localitylineedit"));
    QVERIFY(localitylineedit);

    QLabel *regionlabel = w.findChild<QLabel *>(QStringLiteral("regionlabel"));
    QVERIFY(regionlabel);
    KLineEdit *regionlineedit = w.findChild<KLineEdit *>(QStringLiteral("regionlineedit"));
    QVERIFY(regionlineedit);

    QLabel *postalcodelabel = w.findChild<QLabel *>(QStringLiteral("postalcodelabel"));
    QVERIFY(postalcodelabel);
    KLineEdit *postalcodelineedit = w.findChild<KLineEdit *>(QStringLiteral("postalcodelineedit"));
    QVERIFY(postalcodelineedit);

    QLabel *countrylabel = w.findChild<QLabel *>(QStringLiteral("countrylabel"));
    QVERIFY(countrylabel);
    KComboBox *countrycombobox = w.findChild<KComboBox *>(QStringLiteral("countrycombobox"));
    QVERIFY(countrycombobox);

    QCheckBox *preferredcheckbox = w.findChild<QCheckBox *>(QStringLiteral("preferredcheckbox"));
    QVERIFY(preferredcheckbox);

    QPushButton *addbuttonaddress = w.findChild<QPushButton *>(QStringLiteral("addbuttonaddress"));
    QVERIFY(addbuttonaddress);

    QPushButton *modifybuttonaddress = w.findChild<QPushButton *>(QStringLiteral("modifybuttonaddress"));
    QVERIFY(modifybuttonaddress);

    QPushButton *cancelbuttonaddress = w.findChild<QPushButton *>(QStringLiteral("cancelbuttonaddress"));
    QVERIFY(cancelbuttonaddress);
}

void AddressLocationWidgetTest::shouldChangeReadOnlyStatus()
{
    AddressLocationWidget w;

    w.setReadOnly(true);
    SelectAddressTypeComboBox *typeAddress = w.findChild<SelectAddressTypeComboBox *>(QStringLiteral("typeaddress"));
    QCOMPARE(typeAddress->isEnabled(), false);
    KLineEdit *mStreetEdit = w.findChild<KLineEdit *>(QStringLiteral("streetlineedit"));
    QCOMPARE(mStreetEdit->isReadOnly(), true);
    KLineEdit *postofficeboxlineedit = w.findChild<KLineEdit *>(QStringLiteral("postofficeboxlineedit"));
    QCOMPARE(postofficeboxlineedit->isReadOnly(), true);
    KLineEdit *localitylineedit = w.findChild<KLineEdit *>(QStringLiteral("localitylineedit"));
    QCOMPARE(localitylineedit->isReadOnly(), true);
    KLineEdit *regionlineedit = w.findChild<KLineEdit *>(QStringLiteral("regionlineedit"));
    QCOMPARE(regionlineedit->isReadOnly(), true);
    KLineEdit *postalcodelineedit = w.findChild<KLineEdit *>(QStringLiteral("postalcodelineedit"));
    QCOMPARE(postalcodelineedit->isReadOnly(), true);
    KComboBox *countrycombobox = w.findChild<KComboBox *>(QStringLiteral("countrycombobox"));
    QCOMPARE(countrycombobox->isEnabled(), false);
    QCheckBox *preferredcheckbox = w.findChild<QCheckBox *>(QStringLiteral("preferredcheckbox"));
    QCOMPARE(preferredcheckbox->isEnabled(), false);
    QPushButton *addbuttonaddress = w.findChild<QPushButton *>(QStringLiteral("addbuttonaddress"));
    QCOMPARE(addbuttonaddress->isEnabled(), false);
    QPushButton *modifybuttonaddress = w.findChild<QPushButton *>(QStringLiteral("modifybuttonaddress"));
    QCOMPARE(modifybuttonaddress->isEnabled(), false);
    QPushButton *cancelbuttonaddress = w.findChild<QPushButton *>(QStringLiteral("cancelbuttonaddress"));
    QCOMPARE(cancelbuttonaddress->isEnabled(), false);
}

QTEST_MAIN(AddressLocationWidgetTest)

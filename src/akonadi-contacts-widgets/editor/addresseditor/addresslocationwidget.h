/*
    This file is part of Contact Editor.

    SPDX-FileCopyrightText: 2016 eyeOS S.L.U., a Telefonica company, sales@eyeos.com
    SPDX-FileCopyrightText: 2016-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <KContacts/Address>
#include <QWidget>

class QStackedWidget;
class QCheckBox;
class KLineEdit;
class QComboBox;
class QPushButton;

namespace Akonadi
{
class SelectAddressTypeComboBox;

class AddressLocationWidget : public QWidget
{
    Q_OBJECT
public:
    enum Mode {
        CreateAddress = 0,
        ModifyAddress
    };

    explicit AddressLocationWidget(QWidget *parent = nullptr);
    ~AddressLocationWidget() override;

    void setAddress(const KContacts::Address &address);
    [[nodiscard]] KContacts::Address address() const;

    void slotModifyAddress(const KContacts::Address &address, int currentIndex);

    void clear();

    void setReadOnly(bool readOnly);

    [[nodiscard]] bool wasChanged() const;
Q_SIGNALS:
    void addNewAddress(const KContacts::Address &address);
    void updateAddress(const KContacts::Address &address, int index);
    void updateAddressCanceled();
    void removeAddress(int index);

private:
    void slotChanged();
    void slotAddAddress();
    void slotUpdateAddress();
    void slotRemoveAddress();
    void slotCancelModifyAddress();
    void fillCountryCombo();
    void switchMode();
    void reset();
    KContacts::Address mAddress;
    QCheckBox *mPreferredCheckBox = nullptr;
    KLineEdit *mPOBoxEdit = nullptr;
    KLineEdit *mLocalityEdit = nullptr;
    KLineEdit *mRegionEdit = nullptr;
    KLineEdit *mPostalCodeEdit = nullptr;
    KLineEdit *mStreetEdit = nullptr;
    QComboBox *mCountryCombo = nullptr;
    QPushButton *mAddAddress = nullptr;
    QPushButton *mRemoveAddress = nullptr;
    QPushButton *mModifyAddress = nullptr;
    QPushButton *mCancelAddress = nullptr;
    QStackedWidget *mButtonStack = nullptr;
    SelectAddressTypeComboBox *mTypeCombo = nullptr;
    int mCurrentAddress = -1;
    Mode mCurrentMode;
    bool mWasChanged = false;
};

}

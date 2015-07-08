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

#ifndef PHONEEDITWIDGET_H
#define PHONEEDITWIDGET_H

#include <QtCore/QList>

#include <kcontacts/addressee.h>
#include <KComboBox>
#include <qdialog.h>

class KLineEdit;

class QButtonGroup;
class QCheckBox;
class QScrollArea;
class QSignalMapper;
class QVBoxLayout;

/**
 * @short A combobox to select a phone number type.
 */
class PhoneTypeCombo : public KComboBox
{
    Q_OBJECT

public:
    /**
     * Creates a phone type combo.
     *
     * @param parent The parent widget.
     */
    explicit PhoneTypeCombo(QWidget *parent = Q_NULLPTR);

    /**
     * Destroys the phone type combo.
     */
    ~PhoneTypeCombo();

    /**
     * Sets the phone number @p type that shall be selected.
     */
    void setType(KContacts::PhoneNumber::Type type);

    /**
     * Returns the selected phone number type.
     */
    KContacts::PhoneNumber::Type type() const;

private Q_SLOTS:
    void selected(int);
    void otherSelected();

private:
    void update();

    KContacts::PhoneNumber::Type mType;
    int mLastSelected;
    QList<int> mTypeList;
};

/**
 * A widget that provides selectors for the type
 * and number of a phone number entry.
 */
class PhoneNumberWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new phone number widget.
     *
     * @param parent The parent widget.
     */
    explicit PhoneNumberWidget(QWidget *parent = Q_NULLPTR);

    /**
     * Sets the phone @p number of the widget.
     */
    void setNumber(const KContacts::PhoneNumber &number);

    /**
     * Returns the phone number of the widget.
     */
    KContacts::PhoneNumber number() const;

    /**
     * Sets the widget to @p readOnly mode.
     */
    void setReadOnly(bool readOnly);

Q_SIGNALS:
    void modified();

private:
    PhoneTypeCombo *mTypeCombo;
    KLineEdit *mNumberEdit;
    KContacts::PhoneNumber mNumber;
};

/**
 * A widgets that groups together a list of PhoneNumberWidgets
 */
class PhoneNumberListWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new phone number list widget.
     *
     * @param parent The parent widget.
     */
    explicit PhoneNumberListWidget(QWidget *parent = Q_NULLPTR);

    /**
     * Destroys the phone number list widget.
     */
    ~PhoneNumberListWidget();

    /**
     * Sets the @p list of phone numbers the widget shall show.
     */
    void setPhoneNumbers(const KContacts::PhoneNumber::List &list);

    /**
     * Returns the list of phone numbers.
     */
    KContacts::PhoneNumber::List phoneNumbers() const;

    /**
     * Sets the widget to @p readOnly mode.
     */
    void setReadOnly(bool readOnly);

    /**
     * Returns the number of phone numbers available.
     */
    int phoneNumberCount() const;

public Q_SLOTS:
    /**
     * Adds a new phone number widget to this widget.
     */
    void add();

    /**
     * Removes the last phone number widget from this widget.
     */
    void remove();

private Q_SLOTS:
    void changed(int);

private:
    void recreateNumberWidgets();

    KContacts::PhoneNumber::List mPhoneNumberList;
    QList<PhoneNumberWidget *> mWidgets;

    QVBoxLayout *mWidgetLayout;

    bool mReadOnly;
    QSignalMapper *mMapper;
};

/**
 * @short A widget for editing phone numbers of a contact.
 */
class PhoneEditWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Creates a new phone edit widget.
     *
     * @param parent The parent widget.
     */
    explicit PhoneEditWidget(QWidget *parent = Q_NULLPTR);

    /**
     * Destroys the phone edit widget.
     */
    ~PhoneEditWidget();

    /**
     * Loads the data from @p contact to the widget.
     */
    void loadContact(const KContacts::Addressee &contact);

    /**
     * Stores the data from the widget to the @p contact.
     */
    void storeContact(KContacts::Addressee &contact) const;

    /**
     * Sets the widget to @p readOnly mode.
     */
    void setReadOnly(bool readOnly);

private Q_SLOTS:
    void changed();

private:
    QPushButton *mAddButton;
    QPushButton *mRemoveButton;

    bool mReadOnly;

    QScrollArea *mListScrollArea;
    PhoneNumberListWidget *mPhoneNumberListWidget;
};

/**
 * A dialog for editing phone number types.
 */
class PhoneTypeDialog : public QDialog
{
public:
    /**
     * Creates a new phone type dialog.
     *
     * @param type The initial type of the phone number.
     * @param parent The parent widget.
     */
    explicit PhoneTypeDialog(KContacts::PhoneNumber::Type type, QWidget *parent = Q_NULLPTR);

    /**
     * Returns the selected type.
     */
    KContacts::PhoneNumber::Type type() const;

private:
    KContacts::PhoneNumber::Type mType;
    KContacts::PhoneNumber::TypeList mTypeList;

    QButtonGroup *mGroup;
    QCheckBox *mPreferredBox;
};

#endif

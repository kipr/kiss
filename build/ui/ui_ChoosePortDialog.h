/********************************************************************************
** Form generated from reading UI file 'ChoosePortDialog.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSEPORTDIALOG_H
#define UI_CHOOSEPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ChoosePortDialog
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QGroupBox *ui_portGroupBox;
    QSpacerItem *spacerItem2;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout;
    QPushButton *ui_refreshButton;
    QSpacerItem *spacerItem4;
    QPushButton *ui_addPortButton;
    QSpacerItem *spacerItem5;
    QPushButton *ui_cancelButton;
    QPushButton *ui_okButton;

    void setupUi(QDialog *ChoosePortDialog)
    {
        if (ChoosePortDialog->objectName().isEmpty())
            ChoosePortDialog->setObjectName(QString::fromUtf8("ChoosePortDialog"));
        ChoosePortDialog->setWindowModality(Qt::ApplicationModal);
        ChoosePortDialog->resize(384, 316);
        gridLayout = new QGridLayout(ChoosePortDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacerItem = new QSpacerItem(66, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacerItem, 0, 1, 1, 1);

        spacerItem1 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem1, 1, 0, 1, 1);

        ui_portGroupBox = new QGroupBox(ChoosePortDialog);
        ui_portGroupBox->setObjectName(QString::fromUtf8("ui_portGroupBox"));

        gridLayout->addWidget(ui_portGroupBox, 1, 1, 1, 1);

        spacerItem2 = new QSpacerItem(61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacerItem2, 1, 2, 1, 1);

        spacerItem3 = new QSpacerItem(66, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacerItem3, 2, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        ui_refreshButton = new QPushButton(ChoosePortDialog);
        ui_refreshButton->setObjectName(QString::fromUtf8("ui_refreshButton"));
        ui_refreshButton->setMaximumSize(QSize(81, 16777215));

        hboxLayout->addWidget(ui_refreshButton);

        spacerItem4 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem4);

        ui_addPortButton = new QPushButton(ChoosePortDialog);
        ui_addPortButton->setObjectName(QString::fromUtf8("ui_addPortButton"));

        hboxLayout->addWidget(ui_addPortButton);

        spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem5);

        ui_cancelButton = new QPushButton(ChoosePortDialog);
        ui_cancelButton->setObjectName(QString::fromUtf8("ui_cancelButton"));
        ui_cancelButton->setMinimumSize(QSize(81, 0));
        ui_cancelButton->setMaximumSize(QSize(81, 16777215));

        hboxLayout->addWidget(ui_cancelButton);

        ui_okButton = new QPushButton(ChoosePortDialog);
        ui_okButton->setObjectName(QString::fromUtf8("ui_okButton"));
        ui_okButton->setMinimumSize(QSize(81, 0));
        ui_okButton->setMaximumSize(QSize(81, 16777215));

        hboxLayout->addWidget(ui_okButton);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 3);


        retranslateUi(ChoosePortDialog);
        QObject::connect(ui_okButton, SIGNAL(clicked()), ChoosePortDialog, SLOT(accept()));
        QObject::connect(ui_cancelButton, SIGNAL(clicked()), ChoosePortDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ChoosePortDialog);
    } // setupUi

    void retranslateUi(QDialog *ChoosePortDialog)
    {
        ChoosePortDialog->setWindowTitle(QApplication::translate("ChoosePortDialog", "Port Selection", 0, QApplication::UnicodeUTF8));
        ui_portGroupBox->setTitle(QApplication::translate("ChoosePortDialog", "Port List", 0, QApplication::UnicodeUTF8));
        ui_refreshButton->setText(QApplication::translate("ChoosePortDialog", "Refresh", 0, QApplication::UnicodeUTF8));
        ui_addPortButton->setText(QApplication::translate("ChoosePortDialog", "Add Port", 0, QApplication::UnicodeUTF8));
        ui_cancelButton->setText(QApplication::translate("ChoosePortDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        ui_okButton->setText(QApplication::translate("ChoosePortDialog", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ChoosePortDialog: public Ui_ChoosePortDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSEPORTDIALOG_H

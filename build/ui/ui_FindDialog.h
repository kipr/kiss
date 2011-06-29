/********************************************************************************
** Form generated from reading UI file 'FindDialog.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FindDialog
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *findLabel;
    QLineEdit *ui_findInput;
    QHBoxLayout *hboxLayout1;
    QLabel *label;
    QLineEdit *ui_replaceInput;
    QHBoxLayout *hboxLayout2;
    QCheckBox *ui_matchCaseCheckBox;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout3;
    QPushButton *ui_buttonClose;
    QSpacerItem *spacerItem2;
    QHBoxLayout *hboxLayout4;
    QPushButton *ui_buttonReplace_All;
    QPushButton *ui_buttonReplace;
    QPushButton *ui_buttonFind;

    void setupUi(QDialog *FindDialog)
    {
        if (FindDialog->objectName().isEmpty())
            FindDialog->setObjectName(QString::fromUtf8("FindDialog"));
        FindDialog->resize(374, 156);
        vboxLayout = new QVBoxLayout(FindDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        findLabel = new QLabel(FindDialog);
        findLabel->setObjectName(QString::fromUtf8("findLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(findLabel->sizePolicy().hasHeightForWidth());
        findLabel->setSizePolicy(sizePolicy);
        findLabel->setMinimumSize(QSize(91, 0));

        hboxLayout->addWidget(findLabel);

        ui_findInput = new QLineEdit(FindDialog);
        ui_findInput->setObjectName(QString::fromUtf8("ui_findInput"));

        hboxLayout->addWidget(ui_findInput);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label = new QLabel(FindDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(91, 0));

        hboxLayout1->addWidget(label);

        ui_replaceInput = new QLineEdit(FindDialog);
        ui_replaceInput->setObjectName(QString::fromUtf8("ui_replaceInput"));

        hboxLayout1->addWidget(ui_replaceInput);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        ui_matchCaseCheckBox = new QCheckBox(FindDialog);
        ui_matchCaseCheckBox->setObjectName(QString::fromUtf8("ui_matchCaseCheckBox"));

        hboxLayout2->addWidget(ui_matchCaseCheckBox);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout2);

        spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem1);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        ui_buttonClose = new QPushButton(FindDialog);
        ui_buttonClose->setObjectName(QString::fromUtf8("ui_buttonClose"));
        ui_buttonClose->setAutoDefault(false);

        hboxLayout3->addWidget(ui_buttonClose);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem2);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        ui_buttonReplace_All = new QPushButton(FindDialog);
        ui_buttonReplace_All->setObjectName(QString::fromUtf8("ui_buttonReplace_All"));
        ui_buttonReplace_All->setAutoDefault(false);

        hboxLayout4->addWidget(ui_buttonReplace_All);

        ui_buttonReplace = new QPushButton(FindDialog);
        ui_buttonReplace->setObjectName(QString::fromUtf8("ui_buttonReplace"));
        ui_buttonReplace->setAutoDefault(false);

        hboxLayout4->addWidget(ui_buttonReplace);

        ui_buttonFind = new QPushButton(FindDialog);
        ui_buttonFind->setObjectName(QString::fromUtf8("ui_buttonFind"));

        hboxLayout4->addWidget(ui_buttonFind);


        hboxLayout3->addLayout(hboxLayout4);


        vboxLayout->addLayout(hboxLayout3);


        retranslateUi(FindDialog);
        QObject::connect(ui_buttonClose, SIGNAL(clicked()), FindDialog, SLOT(close()));
        QObject::connect(ui_findInput, SIGNAL(returnPressed()), ui_buttonFind, SLOT(click()));
        QObject::connect(ui_replaceInput, SIGNAL(returnPressed()), ui_buttonReplace, SLOT(click()));

        QMetaObject::connectSlotsByName(FindDialog);
    } // setupUi

    void retranslateUi(QDialog *FindDialog)
    {
        FindDialog->setWindowTitle(QApplication::translate("FindDialog", "Find", 0, QApplication::UnicodeUTF8));
        findLabel->setText(QApplication::translate("FindDialog", "Find:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FindDialog", "Replace With:", 0, QApplication::UnicodeUTF8));
        ui_matchCaseCheckBox->setText(QApplication::translate("FindDialog", "Match Case", 0, QApplication::UnicodeUTF8));
        ui_buttonClose->setText(QApplication::translate("FindDialog", "Close", 0, QApplication::UnicodeUTF8));
        ui_buttonReplace_All->setText(QApplication::translate("FindDialog", "Replace All", 0, QApplication::UnicodeUTF8));
        ui_buttonReplace->setText(QApplication::translate("FindDialog", "Replace", 0, QApplication::UnicodeUTF8));
        ui_buttonFind->setText(QApplication::translate("FindDialog", "Find", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FindDialog: public Ui_FindDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H

/********************************************************************************
** Form generated from reading UI file 'TargetSettingsDialog.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TARGETSETTINGSDIALOG_H
#define UI_TARGETSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TargetSettingsDialog
{
public:
    QVBoxLayout *vboxLayout;
    QTableWidget *ui_settingsTable;
    QHBoxLayout *hboxLayout;
    QPushButton *ui_insertButton;
    QPushButton *ui_deleteButton;
    QSpacerItem *spacerItem;
    QDialogButtonBox *ui_buttonBox;

    void setupUi(QDialog *TargetSettingsDialog)
    {
        if (TargetSettingsDialog->objectName().isEmpty())
            TargetSettingsDialog->setObjectName(QString::fromUtf8("TargetSettingsDialog"));
        TargetSettingsDialog->resize(469, 325);
        vboxLayout = new QVBoxLayout(TargetSettingsDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        ui_settingsTable = new QTableWidget(TargetSettingsDialog);
        ui_settingsTable->setObjectName(QString::fromUtf8("ui_settingsTable"));

        vboxLayout->addWidget(ui_settingsTable);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        ui_insertButton = new QPushButton(TargetSettingsDialog);
        ui_insertButton->setObjectName(QString::fromUtf8("ui_insertButton"));

        hboxLayout->addWidget(ui_insertButton);

        ui_deleteButton = new QPushButton(TargetSettingsDialog);
        ui_deleteButton->setObjectName(QString::fromUtf8("ui_deleteButton"));

        hboxLayout->addWidget(ui_deleteButton);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        ui_buttonBox = new QDialogButtonBox(TargetSettingsDialog);
        ui_buttonBox->setObjectName(QString::fromUtf8("ui_buttonBox"));
        ui_buttonBox->setOrientation(Qt::Horizontal);
        ui_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        hboxLayout->addWidget(ui_buttonBox);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(TargetSettingsDialog);
        QObject::connect(ui_buttonBox, SIGNAL(accepted()), TargetSettingsDialog, SLOT(accept()));
        QObject::connect(ui_buttonBox, SIGNAL(rejected()), TargetSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(TargetSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *TargetSettingsDialog)
    {
        TargetSettingsDialog->setWindowTitle(QApplication::translate("TargetSettingsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        ui_insertButton->setText(QApplication::translate("TargetSettingsDialog", "Insert", 0, QApplication::UnicodeUTF8));
        ui_deleteButton->setText(QApplication::translate("TargetSettingsDialog", "Delete", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TargetSettingsDialog: public Ui_TargetSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TARGETSETTINGSDIALOG_H

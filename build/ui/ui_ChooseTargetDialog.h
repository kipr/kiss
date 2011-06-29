/********************************************************************************
** Form generated from reading UI file 'ChooseTargetDialog.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSETARGETDIALOG_H
#define UI_CHOOSETARGETDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ChooseTargetDialog
{
public:
    QVBoxLayout *vboxLayout;
    QListWidget *ui_listWidget;
    QDialogButtonBox *ui_buttonBox;

    void setupUi(QDialog *ChooseTargetDialog)
    {
        if (ChooseTargetDialog->objectName().isEmpty())
            ChooseTargetDialog->setObjectName(QString::fromUtf8("ChooseTargetDialog"));
        ChooseTargetDialog->setWindowModality(Qt::ApplicationModal);
        ChooseTargetDialog->resize(226, 176);
        vboxLayout = new QVBoxLayout(ChooseTargetDialog);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        ui_listWidget = new QListWidget(ChooseTargetDialog);
        ui_listWidget->setObjectName(QString::fromUtf8("ui_listWidget"));

        vboxLayout->addWidget(ui_listWidget);

        ui_buttonBox = new QDialogButtonBox(ChooseTargetDialog);
        ui_buttonBox->setObjectName(QString::fromUtf8("ui_buttonBox"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ui_buttonBox->sizePolicy().hasHeightForWidth());
        ui_buttonBox->setSizePolicy(sizePolicy);
        ui_buttonBox->setLayoutDirection(Qt::RightToLeft);
        ui_buttonBox->setOrientation(Qt::Horizontal);
        ui_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
        ui_buttonBox->setCenterButtons(true);

        vboxLayout->addWidget(ui_buttonBox);


        retranslateUi(ChooseTargetDialog);
        QObject::connect(ui_buttonBox, SIGNAL(accepted()), ChooseTargetDialog, SLOT(accept()));
        QObject::connect(ui_buttonBox, SIGNAL(rejected()), ChooseTargetDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ChooseTargetDialog);
    } // setupUi

    void retranslateUi(QDialog *ChooseTargetDialog)
    {
        ChooseTargetDialog->setWindowTitle(QApplication::translate("ChooseTargetDialog", "Target Selection", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ChooseTargetDialog: public Ui_ChooseTargetDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSETARGETDIALOG_H

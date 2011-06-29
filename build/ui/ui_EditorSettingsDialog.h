/********************************************************************************
** Form generated from reading UI file 'EditorSettingsDialog.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORSETTINGSDIALOG_H
#define UI_EDITORSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFontComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EditorSettingsDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *ui_fontGroup;
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QFontComboBox *ui_fontBox;
    QSpacerItem *spacerItem;
    QHBoxLayout *hboxLayout1;
    QLabel *label_2;
    QSpinBox *ui_fontSizeSpinBox;
    QSpacerItem *spacerItem1;
    QGroupBox *ui_miscGroup;
    QVBoxLayout *vboxLayout1;
    QCheckBox *ui_callTipsCheckBox;
    QCheckBox *ui_braceMatchingCheckBox;
    QCheckBox *ui_marginLineNumbersCheckBox;
    QSpacerItem *spacerItem2;
    QGroupBox *ui_autoCompletionGroup;
    QVBoxLayout *vboxLayout2;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem3;
    QCheckBox *ui_autoCompletionEnabledCheckBox;
    QSpacerItem *spacerItem4;
    QHBoxLayout *hboxLayout3;
    QLabel *label_3;
    QVBoxLayout *vboxLayout3;
    QCheckBox *ui_autoCompletionAPISourceCheckBox;
    QCheckBox *ui_autoCompletionDocumentSourceCheckBox;
    QHBoxLayout *hboxLayout4;
    QLabel *label_4;
    QSpinBox *ui_autoCompletionThresholdSpinBox;
    QGroupBox *ui_autoIndentGroup;
    QVBoxLayout *vboxLayout4;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacerItem5;
    QCheckBox *ui_autoIndentEnabledCheckBox;
    QSpacerItem *spacerItem6;
    QHBoxLayout *hboxLayout6;
    QLabel *label_5;
    QVBoxLayout *vboxLayout5;
    QRadioButton *ui_autoIndentIntelligentStyleRadioButton;
    QRadioButton *ui_autoIndentMaintainStyleRadioButton;
    QHBoxLayout *hboxLayout7;
    QLabel *label_6;
    QSpinBox *ui_autoIndentWidthSpinBox;
    QDialogButtonBox *ui_buttonBox;

    void setupUi(QDialog *EditorSettingsDialog)
    {
        if (EditorSettingsDialog->objectName().isEmpty())
            EditorSettingsDialog->setObjectName(QString::fromUtf8("EditorSettingsDialog"));
        EditorSettingsDialog->resize(646, 325);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EditorSettingsDialog->sizePolicy().hasHeightForWidth());
        EditorSettingsDialog->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(EditorSettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        ui_fontGroup = new QGroupBox(EditorSettingsDialog);
        ui_fontGroup->setObjectName(QString::fromUtf8("ui_fontGroup"));
        vboxLayout = new QVBoxLayout(ui_fontGroup);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label = new QLabel(ui_fontGroup);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);

        hboxLayout->addWidget(label);

        ui_fontBox = new QFontComboBox(ui_fontGroup);
        ui_fontBox->setObjectName(QString::fromUtf8("ui_fontBox"));
        QFont font1;
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        ui_fontBox->setFont(font1);

        hboxLayout->addWidget(ui_fontBox);

        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label_2 = new QLabel(ui_fontGroup);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(50, 0));
        QFont font2;
        font2.setPointSize(10);
        label_2->setFont(font2);

        hboxLayout1->addWidget(label_2);

        ui_fontSizeSpinBox = new QSpinBox(ui_fontGroup);
        ui_fontSizeSpinBox->setObjectName(QString::fromUtf8("ui_fontSizeSpinBox"));
        ui_fontSizeSpinBox->setMinimum(1);
        ui_fontSizeSpinBox->setMaximum(26);

        hboxLayout1->addWidget(ui_fontSizeSpinBox);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);


        vboxLayout->addLayout(hboxLayout1);


        gridLayout->addWidget(ui_fontGroup, 0, 0, 1, 2);

        ui_miscGroup = new QGroupBox(EditorSettingsDialog);
        ui_miscGroup->setObjectName(QString::fromUtf8("ui_miscGroup"));
        vboxLayout1 = new QVBoxLayout(ui_miscGroup);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        ui_callTipsCheckBox = new QCheckBox(ui_miscGroup);
        ui_callTipsCheckBox->setObjectName(QString::fromUtf8("ui_callTipsCheckBox"));

        vboxLayout1->addWidget(ui_callTipsCheckBox);

        ui_braceMatchingCheckBox = new QCheckBox(ui_miscGroup);
        ui_braceMatchingCheckBox->setObjectName(QString::fromUtf8("ui_braceMatchingCheckBox"));

        vboxLayout1->addWidget(ui_braceMatchingCheckBox);

        ui_marginLineNumbersCheckBox = new QCheckBox(ui_miscGroup);
        ui_marginLineNumbersCheckBox->setObjectName(QString::fromUtf8("ui_marginLineNumbersCheckBox"));

        vboxLayout1->addWidget(ui_marginLineNumbersCheckBox);

        spacerItem2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout1->addItem(spacerItem2);


        gridLayout->addWidget(ui_miscGroup, 0, 2, 2, 1);

        ui_autoCompletionGroup = new QGroupBox(EditorSettingsDialog);
        ui_autoCompletionGroup->setObjectName(QString::fromUtf8("ui_autoCompletionGroup"));
        vboxLayout2 = new QVBoxLayout(ui_autoCompletionGroup);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacerItem3 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem3);

        ui_autoCompletionEnabledCheckBox = new QCheckBox(ui_autoCompletionGroup);
        ui_autoCompletionEnabledCheckBox->setObjectName(QString::fromUtf8("ui_autoCompletionEnabledCheckBox"));

        hboxLayout2->addWidget(ui_autoCompletionEnabledCheckBox);

        spacerItem4 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem4);


        vboxLayout2->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label_3 = new QLabel(ui_autoCompletionGroup);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMinimumSize(QSize(66, 0));

        hboxLayout3->addWidget(label_3);

        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        ui_autoCompletionAPISourceCheckBox = new QCheckBox(ui_autoCompletionGroup);
        ui_autoCompletionAPISourceCheckBox->setObjectName(QString::fromUtf8("ui_autoCompletionAPISourceCheckBox"));

        vboxLayout3->addWidget(ui_autoCompletionAPISourceCheckBox);

        ui_autoCompletionDocumentSourceCheckBox = new QCheckBox(ui_autoCompletionGroup);
        ui_autoCompletionDocumentSourceCheckBox->setObjectName(QString::fromUtf8("ui_autoCompletionDocumentSourceCheckBox"));

        vboxLayout3->addWidget(ui_autoCompletionDocumentSourceCheckBox);


        hboxLayout3->addLayout(vboxLayout3);


        vboxLayout2->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        label_4 = new QLabel(ui_autoCompletionGroup);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);

        hboxLayout4->addWidget(label_4);

        ui_autoCompletionThresholdSpinBox = new QSpinBox(ui_autoCompletionGroup);
        ui_autoCompletionThresholdSpinBox->setObjectName(QString::fromUtf8("ui_autoCompletionThresholdSpinBox"));
        ui_autoCompletionThresholdSpinBox->setMinimum(1);
        ui_autoCompletionThresholdSpinBox->setMaximum(20);

        hboxLayout4->addWidget(ui_autoCompletionThresholdSpinBox);


        vboxLayout2->addLayout(hboxLayout4);


        gridLayout->addWidget(ui_autoCompletionGroup, 1, 0, 1, 1);

        ui_autoIndentGroup = new QGroupBox(EditorSettingsDialog);
        ui_autoIndentGroup->setObjectName(QString::fromUtf8("ui_autoIndentGroup"));
        vboxLayout4 = new QVBoxLayout(ui_autoIndentGroup);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        spacerItem5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout5->addItem(spacerItem5);

        ui_autoIndentEnabledCheckBox = new QCheckBox(ui_autoIndentGroup);
        ui_autoIndentEnabledCheckBox->setObjectName(QString::fromUtf8("ui_autoIndentEnabledCheckBox"));

        hboxLayout5->addWidget(ui_autoIndentEnabledCheckBox);

        spacerItem6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout5->addItem(spacerItem6);


        vboxLayout4->addLayout(hboxLayout5);

        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        label_5 = new QLabel(ui_autoIndentGroup);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(83, 0));

        hboxLayout6->addWidget(label_5);

        vboxLayout5 = new QVBoxLayout();
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        ui_autoIndentIntelligentStyleRadioButton = new QRadioButton(ui_autoIndentGroup);
        ui_autoIndentIntelligentStyleRadioButton->setObjectName(QString::fromUtf8("ui_autoIndentIntelligentStyleRadioButton"));

        vboxLayout5->addWidget(ui_autoIndentIntelligentStyleRadioButton);

        ui_autoIndentMaintainStyleRadioButton = new QRadioButton(ui_autoIndentGroup);
        ui_autoIndentMaintainStyleRadioButton->setObjectName(QString::fromUtf8("ui_autoIndentMaintainStyleRadioButton"));

        vboxLayout5->addWidget(ui_autoIndentMaintainStyleRadioButton);


        hboxLayout6->addLayout(vboxLayout5);


        vboxLayout4->addLayout(hboxLayout6);

        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        label_6 = new QLabel(ui_autoIndentGroup);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        hboxLayout7->addWidget(label_6);

        ui_autoIndentWidthSpinBox = new QSpinBox(ui_autoIndentGroup);
        ui_autoIndentWidthSpinBox->setObjectName(QString::fromUtf8("ui_autoIndentWidthSpinBox"));
        ui_autoIndentWidthSpinBox->setMinimum(1);
        ui_autoIndentWidthSpinBox->setMaximum(20);

        hboxLayout7->addWidget(ui_autoIndentWidthSpinBox);


        vboxLayout4->addLayout(hboxLayout7);


        gridLayout->addWidget(ui_autoIndentGroup, 1, 1, 1, 1);

        ui_buttonBox = new QDialogButtonBox(EditorSettingsDialog);
        ui_buttonBox->setObjectName(QString::fromUtf8("ui_buttonBox"));
        ui_buttonBox->setOrientation(Qt::Horizontal);
        ui_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(ui_buttonBox, 2, 0, 1, 3);


        retranslateUi(EditorSettingsDialog);
        QObject::connect(ui_buttonBox, SIGNAL(accepted()), EditorSettingsDialog, SLOT(accept()));
        QObject::connect(ui_buttonBox, SIGNAL(rejected()), EditorSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EditorSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *EditorSettingsDialog)
    {
        EditorSettingsDialog->setWindowTitle(QApplication::translate("EditorSettingsDialog", "Settings", 0, QApplication::UnicodeUTF8));
        ui_fontGroup->setTitle(QApplication::translate("EditorSettingsDialog", "Font", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EditorSettingsDialog", "Default:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EditorSettingsDialog", "Size: ", 0, QApplication::UnicodeUTF8));
        ui_miscGroup->setTitle(QApplication::translate("EditorSettingsDialog", "Misc", 0, QApplication::UnicodeUTF8));
        ui_callTipsCheckBox->setText(QApplication::translate("EditorSettingsDialog", "Call Tips", 0, QApplication::UnicodeUTF8));
        ui_braceMatchingCheckBox->setText(QApplication::translate("EditorSettingsDialog", "Brace Matching", 0, QApplication::UnicodeUTF8));
        ui_marginLineNumbersCheckBox->setText(QApplication::translate("EditorSettingsDialog", "Margin Line Numbers", 0, QApplication::UnicodeUTF8));
        ui_autoCompletionGroup->setTitle(QApplication::translate("EditorSettingsDialog", "Auto Completion", 0, QApplication::UnicodeUTF8));
        ui_autoCompletionEnabledCheckBox->setText(QApplication::translate("EditorSettingsDialog", "Enabled", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("EditorSettingsDialog", "Source:", 0, QApplication::UnicodeUTF8));
        ui_autoCompletionAPISourceCheckBox->setText(QApplication::translate("EditorSettingsDialog", "API", 0, QApplication::UnicodeUTF8));
        ui_autoCompletionDocumentSourceCheckBox->setText(QApplication::translate("EditorSettingsDialog", "Document", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("EditorSettingsDialog", "Threshold:", 0, QApplication::UnicodeUTF8));
        ui_autoIndentGroup->setTitle(QApplication::translate("EditorSettingsDialog", "Auto Indent", 0, QApplication::UnicodeUTF8));
        ui_autoIndentEnabledCheckBox->setText(QApplication::translate("EditorSettingsDialog", "Enabled", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("EditorSettingsDialog", "Style:", 0, QApplication::UnicodeUTF8));
        ui_autoIndentIntelligentStyleRadioButton->setText(QApplication::translate("EditorSettingsDialog", "Intelligent", 0, QApplication::UnicodeUTF8));
        ui_autoIndentMaintainStyleRadioButton->setText(QApplication::translate("EditorSettingsDialog", "Maintain", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("EditorSettingsDialog", "Indent Width:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EditorSettingsDialog: public Ui_EditorSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORSETTINGSDIALOG_H

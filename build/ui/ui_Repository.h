/********************************************************************************
** Form generated from reading UI file 'Repository.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPOSITORY_H
#define UI_REPOSITORY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Repository
{
public:
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QListWidget *ui_list;
    QHBoxLayout *horizontalLayout;
    QPushButton *ui_source;
    QSpacerItem *horizontalSpacer;
    QPushButton *ui_mark;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QListWidget *ui_stagedList;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *ui_unmark;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_3;
    QListWidget *ui_installList;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *ui_uninstall;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QProgressBar *ui_progressBar;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *ui_begin;
    QListWidget *ui_log;

    void setupUi(QWidget *Repository)
    {
        if (Repository->objectName().isEmpty())
            Repository->setObjectName(QString::fromUtf8("Repository"));
        Repository->resize(905, 524);
        verticalLayout_4 = new QVBoxLayout(Repository);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        splitter = new QSplitter(Repository);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ui_list = new QListWidget(groupBox);
        ui_list->setObjectName(QString::fromUtf8("ui_list"));

        verticalLayout->addWidget(ui_list);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ui_source = new QPushButton(groupBox);
        ui_source->setObjectName(QString::fromUtf8("ui_source"));

        horizontalLayout->addWidget(ui_source);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ui_mark = new QPushButton(groupBox);
        ui_mark->setObjectName(QString::fromUtf8("ui_mark"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/tick.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui_mark->setIcon(icon);

        horizontalLayout->addWidget(ui_mark);


        verticalLayout->addLayout(horizontalLayout);

        splitter->addWidget(groupBox);
        groupBox_2 = new QGroupBox(splitter);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ui_stagedList = new QListWidget(groupBox_2);
        ui_stagedList->setObjectName(QString::fromUtf8("ui_stagedList"));
        ui_stagedList->setIconSize(QSize(16, 16));
        ui_stagedList->setFlow(QListView::TopToBottom);
        ui_stagedList->setViewMode(QListView::ListMode);

        verticalLayout_2->addWidget(ui_stagedList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        ui_unmark = new QPushButton(groupBox_2);
        ui_unmark->setObjectName(QString::fromUtf8("ui_unmark"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/shortcuts/file/icon_set/icons/cross.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui_unmark->setIcon(icon1);

        horizontalLayout_2->addWidget(ui_unmark);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        splitter->addWidget(groupBox_2);
        groupBox_3 = new QGroupBox(splitter);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_3 = new QVBoxLayout(groupBox_3);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        ui_installList = new QListWidget(groupBox_3);
        ui_installList->setObjectName(QString::fromUtf8("ui_installList"));

        verticalLayout_3->addWidget(ui_installList);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        ui_uninstall = new QPushButton(groupBox_3);
        ui_uninstall->setObjectName(QString::fromUtf8("ui_uninstall"));
        ui_uninstall->setIcon(icon1);

        horizontalLayout_3->addWidget(ui_uninstall);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        verticalLayout_3->addLayout(horizontalLayout_3);

        splitter->addWidget(groupBox_3);

        verticalLayout_4->addWidget(splitter);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        ui_progressBar = new QProgressBar(Repository);
        ui_progressBar->setObjectName(QString::fromUtf8("ui_progressBar"));
        ui_progressBar->setEnabled(false);
        ui_progressBar->setValue(0);

        horizontalLayout_4->addWidget(ui_progressBar);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        ui_begin = new QPushButton(Repository);
        ui_begin->setObjectName(QString::fromUtf8("ui_begin"));

        horizontalLayout_4->addWidget(ui_begin);


        verticalLayout_4->addLayout(horizontalLayout_4);

        ui_log = new QListWidget(Repository);
        ui_log->setObjectName(QString::fromUtf8("ui_log"));

        verticalLayout_4->addWidget(ui_log);

        verticalLayout_4->setStretch(0, 10);
        verticalLayout_4->setStretch(2, 3);

        retranslateUi(Repository);

        QMetaObject::connectSlotsByName(Repository);
    } // setupUi

    void retranslateUi(QWidget *Repository)
    {
        Repository->setWindowTitle(QApplication::translate("Repository", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Repository", "Available", 0, QApplication::UnicodeUTF8));
        ui_source->setText(QApplication::translate("Repository", "Source", 0, QApplication::UnicodeUTF8));
        ui_mark->setText(QApplication::translate("Repository", "Install", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Repository", "Staged", 0, QApplication::UnicodeUTF8));
        ui_unmark->setText(QApplication::translate("Repository", "Unstage", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Repository", "Installed", 0, QApplication::UnicodeUTF8));
        ui_uninstall->setText(QApplication::translate("Repository", "Uninstall", 0, QApplication::UnicodeUTF8));
        ui_begin->setText(QApplication::translate("Repository", "Commit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Repository: public Ui_Repository {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPOSITORY_H

/********************************************************************************
** Form generated from reading UI file 'Debugger.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGGER_H
#define UI_DEBUGGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Debugger
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QFrame *frame;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *ui_run;
    QPushButton *ui_step;
    QPushButton *ui_pause;
    QPushButton *ui_stop;
    QSpacerItem *horizontalSpacer;
    QPushButton *ui_clear;
    QTextEdit *ui_consoleOut;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *ui_consoleIn;
    QPushButton *ui_consoleEnter;
    QTabWidget *ui_tabWidget;
    QWidget *ui_stackTab;
    QVBoxLayout *verticalLayout_4;
    QListWidget *ui_stack;
    QWidget *ui_threadTab;
    QVBoxLayout *verticalLayout_2;
    QListWidget *ui_threads;
    QWidget *ui_variableTab;
    QVBoxLayout *verticalLayout_7;
    QListWidget *ui_variables;
    QWidget *ui_breakpointTab;
    QVBoxLayout *verticalLayout_3;
    QListWidget *ui_breakpoints;
    QWidget *ui_librariesTab;
    QVBoxLayout *verticalLayout_6;
    QListWidget *ui_libs;

    void setupUi(QWidget *Debugger)
    {
        if (Debugger->objectName().isEmpty())
            Debugger->setObjectName(QString::fromUtf8("Debugger"));
        Debugger->resize(612, 527);
        verticalLayout = new QVBoxLayout(Debugger);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(Debugger);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        frame = new QFrame(splitter);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        verticalLayout_5 = new QVBoxLayout(frame);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        horizontalLayout->setContentsMargins(0, 0, -1, -1);
        ui_run = new QPushButton(frame);
        ui_run->setObjectName(QString::fromUtf8("ui_run"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ui_run->sizePolicy().hasHeightForWidth());
        ui_run->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/debugger/icon_set/icons/control_play.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui_run->setIcon(icon);
        ui_run->setFlat(false);

        horizontalLayout->addWidget(ui_run);

        ui_step = new QPushButton(frame);
        ui_step->setObjectName(QString::fromUtf8("ui_step"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/debugger/icon_set/icons/control_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui_step->setIcon(icon1);

        horizontalLayout->addWidget(ui_step);

        ui_pause = new QPushButton(frame);
        ui_pause->setObjectName(QString::fromUtf8("ui_pause"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/debugger/icon_set/icons/control_pause.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui_pause->setIcon(icon2);

        horizontalLayout->addWidget(ui_pause);

        ui_stop = new QPushButton(frame);
        ui_stop->setObjectName(QString::fromUtf8("ui_stop"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/debugger/icon_set/icons/control_stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui_stop->setIcon(icon3);

        horizontalLayout->addWidget(ui_stop);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ui_clear = new QPushButton(frame);
        ui_clear->setObjectName(QString::fromUtf8("ui_clear"));

        horizontalLayout->addWidget(ui_clear);


        verticalLayout_5->addLayout(horizontalLayout);

        ui_consoleOut = new QTextEdit(frame);
        ui_consoleOut->setObjectName(QString::fromUtf8("ui_consoleOut"));
        ui_consoleOut->setReadOnly(true);

        verticalLayout_5->addWidget(ui_consoleOut);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ui_consoleIn = new QLineEdit(frame);
        ui_consoleIn->setObjectName(QString::fromUtf8("ui_consoleIn"));

        horizontalLayout_2->addWidget(ui_consoleIn);

        ui_consoleEnter = new QPushButton(frame);
        ui_consoleEnter->setObjectName(QString::fromUtf8("ui_consoleEnter"));

        horizontalLayout_2->addWidget(ui_consoleEnter);


        verticalLayout_5->addLayout(horizontalLayout_2);

        splitter->addWidget(frame);
        ui_tabWidget = new QTabWidget(splitter);
        ui_tabWidget->setObjectName(QString::fromUtf8("ui_tabWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ui_tabWidget->sizePolicy().hasHeightForWidth());
        ui_tabWidget->setSizePolicy(sizePolicy1);
        ui_tabWidget->setMaximumSize(QSize(16777215, 16777215));
        ui_tabWidget->setTabShape(QTabWidget::Rounded);
        ui_tabWidget->setMovable(true);
        ui_stackTab = new QWidget();
        ui_stackTab->setObjectName(QString::fromUtf8("ui_stackTab"));
        verticalLayout_4 = new QVBoxLayout(ui_stackTab);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        ui_stack = new QListWidget(ui_stackTab);
        ui_stack->setObjectName(QString::fromUtf8("ui_stack"));

        verticalLayout_4->addWidget(ui_stack);

        ui_tabWidget->addTab(ui_stackTab, QString());
        ui_threadTab = new QWidget();
        ui_threadTab->setObjectName(QString::fromUtf8("ui_threadTab"));
        verticalLayout_2 = new QVBoxLayout(ui_threadTab);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        ui_threads = new QListWidget(ui_threadTab);
        ui_threads->setObjectName(QString::fromUtf8("ui_threads"));

        verticalLayout_2->addWidget(ui_threads);

        ui_tabWidget->addTab(ui_threadTab, QString());
        ui_variableTab = new QWidget();
        ui_variableTab->setObjectName(QString::fromUtf8("ui_variableTab"));
        verticalLayout_7 = new QVBoxLayout(ui_variableTab);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        ui_variables = new QListWidget(ui_variableTab);
        ui_variables->setObjectName(QString::fromUtf8("ui_variables"));

        verticalLayout_7->addWidget(ui_variables);

        ui_tabWidget->addTab(ui_variableTab, QString());
        ui_breakpointTab = new QWidget();
        ui_breakpointTab->setObjectName(QString::fromUtf8("ui_breakpointTab"));
        verticalLayout_3 = new QVBoxLayout(ui_breakpointTab);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        ui_breakpoints = new QListWidget(ui_breakpointTab);
        ui_breakpoints->setObjectName(QString::fromUtf8("ui_breakpoints"));

        verticalLayout_3->addWidget(ui_breakpoints);

        ui_tabWidget->addTab(ui_breakpointTab, QString());
        ui_librariesTab = new QWidget();
        ui_librariesTab->setObjectName(QString::fromUtf8("ui_librariesTab"));
        verticalLayout_6 = new QVBoxLayout(ui_librariesTab);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        ui_libs = new QListWidget(ui_librariesTab);
        ui_libs->setObjectName(QString::fromUtf8("ui_libs"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ui_libs->sizePolicy().hasHeightForWidth());
        ui_libs->setSizePolicy(sizePolicy2);

        verticalLayout_6->addWidget(ui_libs);

        ui_tabWidget->addTab(ui_librariesTab, QString());
        splitter->addWidget(ui_tabWidget);

        verticalLayout->addWidget(splitter);


        retranslateUi(Debugger);
        QObject::connect(ui_consoleIn, SIGNAL(returnPressed()), ui_consoleEnter, SLOT(click()));
        QObject::connect(ui_clear, SIGNAL(clicked()), ui_consoleOut, SLOT(clear()));

        ui_tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Debugger);
    } // setupUi

    void retranslateUi(QWidget *Debugger)
    {
        Debugger->setWindowTitle(QApplication::translate("Debugger", "Debugger", 0, QApplication::UnicodeUTF8));
        ui_run->setText(QApplication::translate("Debugger", "Run", 0, QApplication::UnicodeUTF8));
        ui_step->setText(QApplication::translate("Debugger", "Step", 0, QApplication::UnicodeUTF8));
        ui_pause->setText(QApplication::translate("Debugger", "Pause", 0, QApplication::UnicodeUTF8));
        ui_stop->setText(QApplication::translate("Debugger", "Stop", 0, QApplication::UnicodeUTF8));
        ui_clear->setText(QApplication::translate("Debugger", "Clear", 0, QApplication::UnicodeUTF8));
        ui_consoleEnter->setText(QApplication::translate("Debugger", "Enter", 0, QApplication::UnicodeUTF8));
        ui_tabWidget->setTabText(ui_tabWidget->indexOf(ui_stackTab), QApplication::translate("Debugger", "Stack", 0, QApplication::UnicodeUTF8));
        ui_tabWidget->setTabText(ui_tabWidget->indexOf(ui_threadTab), QApplication::translate("Debugger", "Threads", 0, QApplication::UnicodeUTF8));
        ui_tabWidget->setTabText(ui_tabWidget->indexOf(ui_variableTab), QApplication::translate("Debugger", "Variables", 0, QApplication::UnicodeUTF8));
        ui_tabWidget->setTabText(ui_tabWidget->indexOf(ui_breakpointTab), QApplication::translate("Debugger", "Breakpoints", 0, QApplication::UnicodeUTF8));
        ui_tabWidget->setTabText(ui_tabWidget->indexOf(ui_librariesTab), QApplication::translate("Debugger", "Libraries", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Debugger: public Ui_Debugger {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGGER_H

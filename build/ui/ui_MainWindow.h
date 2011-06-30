/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Thu Jun 30 10:57:36 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionAbout;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionEditor_Settings;
    QAction *actionTarget_Settings;
    QAction *actionSimulate;
    QAction *actionClose;
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionNext;
    QAction *actionPrevious;
    QAction *actionNewProject;
    QAction *actionManagePackages;
    QAction *actionInstallLocalPackage;
    QAction *actionHideErrors;
    QWidget *ui_centralwidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTabWidget *ui_tabWidget;
    QWidget *tab;
    QTabWidget *ui_errorView;
    QWidget *tab_2;
    QStatusBar *ui_statusbar;
    QMenuBar *ui_menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QMenu *menuSettings;
    QToolBar *ui_toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->resize(996, 609);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionQuit->setMenuRole(QAction::QuitRole);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        actionEditor_Settings = new QAction(MainWindow);
        actionEditor_Settings->setObjectName(QString::fromUtf8("actionEditor_Settings"));
        actionEditor_Settings->setMenuRole(QAction::PreferencesRole);
        actionTarget_Settings = new QAction(MainWindow);
        actionTarget_Settings->setObjectName(QString::fromUtf8("actionTarget_Settings"));
        actionSimulate = new QAction(MainWindow);
        actionSimulate->setObjectName(QString::fromUtf8("actionSimulate"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/bullet_go.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSimulate->setIcon(icon);
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/shortcuts/file/icon_set/icons/cross.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose->setIcon(icon1);
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/shortcuts/file/icon_set/icons/page_white.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon2);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/shortcuts/file/icon_set/icons/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon3);
        actionNext = new QAction(MainWindow);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        actionPrevious = new QAction(MainWindow);
        actionPrevious->setObjectName(QString::fromUtf8("actionPrevious"));
        actionNewProject = new QAction(MainWindow);
        actionNewProject->setObjectName(QString::fromUtf8("actionNewProject"));
        actionManagePackages = new QAction(MainWindow);
        actionManagePackages->setObjectName(QString::fromUtf8("actionManagePackages"));
        actionInstallLocalPackage = new QAction(MainWindow);
        actionInstallLocalPackage->setObjectName(QString::fromUtf8("actionInstallLocalPackage"));
        actionHideErrors = new QAction(MainWindow);
        actionHideErrors->setObjectName(QString::fromUtf8("actionHideErrors"));
        ui_centralwidget = new QWidget(MainWindow);
        ui_centralwidget->setObjectName(QString::fromUtf8("ui_centralwidget"));
        horizontalLayout = new QHBoxLayout(ui_centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(ui_centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        ui_tabWidget = new QTabWidget(splitter);
        ui_tabWidget->setObjectName(QString::fromUtf8("ui_tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(ui_tabWidget->sizePolicy().hasHeightForWidth());
        ui_tabWidget->setSizePolicy(sizePolicy);
        ui_tabWidget->setTabPosition(QTabWidget::North);
        ui_tabWidget->setElideMode(Qt::ElideRight);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        ui_tabWidget->addTab(tab, QString());
        splitter->addWidget(ui_tabWidget);
        ui_errorView = new QTabWidget(splitter);
        ui_errorView->setObjectName(QString::fromUtf8("ui_errorView"));
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        ui_errorView->addTab(tab_2, QString());
        splitter->addWidget(ui_errorView);

        horizontalLayout->addWidget(splitter);

        horizontalLayout->setStretch(0, 3);
        MainWindow->setCentralWidget(ui_centralwidget);
        ui_statusbar = new QStatusBar(MainWindow);
        ui_statusbar->setObjectName(QString::fromUtf8("ui_statusbar"));
        MainWindow->setStatusBar(ui_statusbar);
        ui_menubar = new QMenuBar(MainWindow);
        ui_menubar->setObjectName(QString::fromUtf8("ui_menubar"));
        ui_menubar->setGeometry(QRect(0, 0, 996, 22));
        menuFile = new QMenu(ui_menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(ui_menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuHelp = new QMenu(ui_menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuSettings = new QMenu(ui_menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        MainWindow->setMenuBar(ui_menubar);
        ui_toolBar = new QToolBar(MainWindow);
        ui_toolBar->setObjectName(QString::fromUtf8("ui_toolBar"));
        ui_toolBar->setIconSize(QSize(16, 16));
        ui_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        MainWindow->addToolBar(Qt::TopToolBarArea, ui_toolBar);

        ui_menubar->addAction(menuFile->menuAction());
        ui_menubar->addAction(menuEdit->menuAction());
        ui_menubar->addAction(menuSettings->menuAction());
        ui_menubar->addAction(menuHelp->menuAction());

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(actionHideErrors, SIGNAL(triggered()), ui_errorView, SLOT(hide()));

        ui_tabWidget->setCurrentIndex(0);
        ui_errorView->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "KIPR's Instructional Software System", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoom_In->setShortcut(QApplication::translate("MainWindow", "Ctrl++", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoom_Out->setShortcut(QApplication::translate("MainWindow", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionEditor_Settings->setText(QApplication::translate("MainWindow", "Editor Settings", 0, QApplication::UnicodeUTF8));
        actionTarget_Settings->setText(QApplication::translate("MainWindow", "Target Settings", 0, QApplication::UnicodeUTF8));
        actionSimulate->setText(QApplication::translate("MainWindow", "Simulate", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("MainWindow", "Ctrl+W", 0, QApplication::UnicodeUTF8));
        actionNew->setText(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
        actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+N", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0, QApplication::UnicodeUTF8));
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("MainWindow", "Next", 0, QApplication::UnicodeUTF8));
        actionNext->setShortcut(QApplication::translate("MainWindow", "Ctrl+Right", 0, QApplication::UnicodeUTF8));
        actionPrevious->setText(QApplication::translate("MainWindow", "Previous", 0, QApplication::UnicodeUTF8));
        actionPrevious->setShortcut(QApplication::translate("MainWindow", "Ctrl+Left", 0, QApplication::UnicodeUTF8));
        actionNewProject->setText(QApplication::translate("MainWindow", "New Project", 0, QApplication::UnicodeUTF8));
        actionNewProject->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionManagePackages->setText(QApplication::translate("MainWindow", "Manage Packages", 0, QApplication::UnicodeUTF8));
        actionInstallLocalPackage->setText(QApplication::translate("MainWindow", "Install Local Package", 0, QApplication::UnicodeUTF8));
        actionHideErrors->setText(QApplication::translate("MainWindow", "Hide Errors", 0, QApplication::UnicodeUTF8));
        ui_tabWidget->setTabText(ui_tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        ui_errorView->setTabText(ui_errorView->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 1", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("MainWindow", "Settings", 0, QApplication::UnicodeUTF8));
        ui_toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

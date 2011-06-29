/********************************************************************************
** Form generated from reading UI file 'SourceFile.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOURCEFILE_H
#define UI_SOURCEFILE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "Qsci/qsciscintilla.h"

QT_BEGIN_NAMESPACE

class Ui_SourceFile
{
public:
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionCompile;
    QAction *actionRun;
    QAction *actionSimulate;
    QAction *actionStop;
    QAction *actionDownload;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionCut;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionManual;
    QAction *actionPrint;
    QAction *actionZoomIn;
    QAction *actionZoomOut;
    QAction *actionResetZoomLevel;
    QAction *actionFind;
    QAction *actionChangeTarget;
    QAction *actionChoosePort;
    QAction *actionDebug;
    QAction *actionToggleBreakpoint;
    QVBoxLayout *verticalLayout;
    QsciScintilla *ui_editor;

    void setupUi(QWidget *SourceFile)
    {
        if (SourceFile->objectName().isEmpty())
            SourceFile->setObjectName(QString::fromUtf8("SourceFile"));
        SourceFile->resize(508, 434);
        actionSave = new QAction(SourceFile);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/shortcuts/file/icon_set/icons/disk.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon);
        actionSaveAs = new QAction(SourceFile);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionCompile = new QAction(SourceFile);
        actionCompile->setObjectName(QString::fromUtf8("actionCompile"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/bricks.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompile->setIcon(icon1);
        actionRun = new QAction(SourceFile);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/bullet_go.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon2);
        actionSimulate = new QAction(SourceFile);
        actionSimulate->setObjectName(QString::fromUtf8("actionSimulate"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/application_go.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSimulate->setIcon(icon3);
        actionStop = new QAction(SourceFile);
        actionStop->setObjectName(QString::fromUtf8("actionStop"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon4);
        actionDownload = new QAction(SourceFile);
        actionDownload->setObjectName(QString::fromUtf8("actionDownload"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/ruby_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDownload->setIcon(icon5);
        actionCopy = new QAction(SourceFile);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/page_white_copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon6);
        actionPaste = new QAction(SourceFile);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/page_white_paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon7);
        actionCut = new QAction(SourceFile);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon8);
        actionUndo = new QAction(SourceFile);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/arrow_undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon9);
        actionRedo = new QAction(SourceFile);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/arrow_redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon10);
        actionManual = new QAction(SourceFile);
        actionManual->setObjectName(QString::fromUtf8("actionManual"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/report.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionManual->setIcon(icon11);
        actionPrint = new QAction(SourceFile);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/shortcuts/file/icon_set/icons/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon12);
        actionZoomIn = new QAction(SourceFile);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        actionZoomOut = new QAction(SourceFile);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        actionResetZoomLevel = new QAction(SourceFile);
        actionResetZoomLevel->setObjectName(QString::fromUtf8("actionResetZoomLevel"));
        actionFind = new QAction(SourceFile);
        actionFind->setObjectName(QString::fromUtf8("actionFind"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/find.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionFind->setIcon(icon13);
        actionChangeTarget = new QAction(SourceFile);
        actionChangeTarget->setObjectName(QString::fromUtf8("actionChangeTarget"));
        actionChoosePort = new QAction(SourceFile);
        actionChoosePort->setObjectName(QString::fromUtf8("actionChoosePort"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/link.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionChoosePort->setIcon(icon14);
        actionDebug = new QAction(SourceFile);
        actionDebug->setObjectName(QString::fromUtf8("actionDebug"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/shortcuts/target/icon_set/icons/bug.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDebug->setIcon(icon15);
        actionToggleBreakpoint = new QAction(SourceFile);
        actionToggleBreakpoint->setObjectName(QString::fromUtf8("actionToggleBreakpoint"));
        actionToggleBreakpoint->setCheckable(true);
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/sourcefile/icon_set/icons/bullet_blue.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionToggleBreakpoint->setIcon(icon16);
        verticalLayout = new QVBoxLayout(SourceFile);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        ui_editor = new QsciScintilla(SourceFile);
        ui_editor->setObjectName(QString::fromUtf8("ui_editor"));

        verticalLayout->addWidget(ui_editor);


        retranslateUi(SourceFile);

        QMetaObject::connectSlotsByName(SourceFile);
    } // setupUi

    void retranslateUi(QWidget *SourceFile)
    {
        SourceFile->setWindowTitle(QApplication::translate("SourceFile", "Form", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("SourceFile", "Save", 0, QApplication::UnicodeUTF8));
        actionSave->setShortcut(QApplication::translate("SourceFile", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionSaveAs->setText(QApplication::translate("SourceFile", "Save As", 0, QApplication::UnicodeUTF8));
        actionSaveAs->setShortcut(QApplication::translate("SourceFile", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionCompile->setText(QApplication::translate("SourceFile", "Compile", 0, QApplication::UnicodeUTF8));
        actionCompile->setShortcut(QApplication::translate("SourceFile", "Ctrl+Shift+C", 0, QApplication::UnicodeUTF8));
        actionRun->setText(QApplication::translate("SourceFile", "Run", 0, QApplication::UnicodeUTF8));
        actionRun->setShortcut(QApplication::translate("SourceFile", "Ctrl+Alt+R", 0, QApplication::UnicodeUTF8));
        actionSimulate->setText(QApplication::translate("SourceFile", "Simulate", 0, QApplication::UnicodeUTF8));
        actionSimulate->setShortcut(QApplication::translate("SourceFile", "Ctrl+Alt+S", 0, QApplication::UnicodeUTF8));
        actionStop->setText(QApplication::translate("SourceFile", "Stop", 0, QApplication::UnicodeUTF8));
        actionStop->setShortcut(QApplication::translate("SourceFile", "Ctrl+Alt+.", 0, QApplication::UnicodeUTF8));
        actionDownload->setText(QApplication::translate("SourceFile", "Download", 0, QApplication::UnicodeUTF8));
        actionDownload->setShortcut(QApplication::translate("SourceFile", "Ctrl+D", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("SourceFile", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("SourceFile", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("SourceFile", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("SourceFile", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("SourceFile", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("SourceFile", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("SourceFile", "Undo", 0, QApplication::UnicodeUTF8));
        actionUndo->setShortcut(QApplication::translate("SourceFile", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("SourceFile", "Redo", 0, QApplication::UnicodeUTF8));
        actionRedo->setShortcut(QApplication::translate("SourceFile", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
        actionManual->setText(QApplication::translate("SourceFile", "Manual", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("SourceFile", "Print", 0, QApplication::UnicodeUTF8));
        actionPrint->setShortcut(QApplication::translate("SourceFile", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("SourceFile", "Zoom In", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setShortcut(QApplication::translate("SourceFile", "Ctrl+=", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setText(QApplication::translate("SourceFile", "Zoom Out", 0, QApplication::UnicodeUTF8));
        actionZoomOut->setShortcut(QApplication::translate("SourceFile", "Ctrl+-", 0, QApplication::UnicodeUTF8));
        actionResetZoomLevel->setText(QApplication::translate("SourceFile", "Reset Zoom Level", 0, QApplication::UnicodeUTF8));
        actionResetZoomLevel->setShortcut(QApplication::translate("SourceFile", "Ctrl+0", 0, QApplication::UnicodeUTF8));
        actionFind->setText(QApplication::translate("SourceFile", "Find", 0, QApplication::UnicodeUTF8));
        actionFind->setShortcut(QApplication::translate("SourceFile", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        actionChangeTarget->setText(QApplication::translate("SourceFile", "Change Target", 0, QApplication::UnicodeUTF8));
        actionChoosePort->setText(QApplication::translate("SourceFile", "Choose Port", 0, QApplication::UnicodeUTF8));
        actionDebug->setText(QApplication::translate("SourceFile", "Debug", 0, QApplication::UnicodeUTF8));
        actionToggleBreakpoint->setText(QApplication::translate("SourceFile", "Toggle Breakpoint", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionToggleBreakpoint->setToolTip(QApplication::translate("SourceFile", "Toggle Breakpoint", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionToggleBreakpoint->setShortcut(QApplication::translate("SourceFile", "Ctrl+Shift+B", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ui_editor->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        ui_editor->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class SourceFile: public Ui_SourceFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOURCEFILE_H

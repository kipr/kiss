/********************************************************************************
** Form generated from reading UI file 'WebTab.ui'
**
** Created: Wed Jun 29 09:40:50 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEBTAB_H
#define UI_WEBTAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtWebKit/QWebView>

QT_BEGIN_NAMESPACE

class Ui_WebTab
{
public:
    QAction *actionBack;
    QAction *actionForward;
    QAction *actionReload;
    QAction *actionGo;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *ui_urlBar;
    QPushButton *ui_goButton;
    QWebView *ui_webView;

    void setupUi(QWidget *WebTab)
    {
        if (WebTab->objectName().isEmpty())
            WebTab->setObjectName(QString::fromUtf8("WebTab"));
        WebTab->resize(901, 650);
        actionBack = new QAction(WebTab);
        actionBack->setObjectName(QString::fromUtf8("actionBack"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/browser/icon_set/icons/arrow_left.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBack->setIcon(icon);
        actionForward = new QAction(WebTab);
        actionForward->setObjectName(QString::fromUtf8("actionForward"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/browser/icon_set/icons/arrow_right.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionForward->setIcon(icon1);
        actionReload = new QAction(WebTab);
        actionReload->setObjectName(QString::fromUtf8("actionReload"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/browser/icon_set/icons/arrow_refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReload->setIcon(icon2);
        actionGo = new QAction(WebTab);
        actionGo->setObjectName(QString::fromUtf8("actionGo"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/browser/icon_set/icons/resultset_next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGo->setIcon(icon3);
        actionCopy = new QAction(WebTab);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/page_white_copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon4);
        actionCut = new QAction(WebTab);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon5);
        actionPaste = new QAction(WebTab);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/shortcuts/edit/icon_set/icons/page_white_paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon6);
        verticalLayout = new QVBoxLayout(WebTab);
        verticalLayout->setSpacing(-1);
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        ui_urlBar = new QLineEdit(WebTab);
        ui_urlBar->setObjectName(QString::fromUtf8("ui_urlBar"));

        horizontalLayout->addWidget(ui_urlBar);

        ui_goButton = new QPushButton(WebTab);
        ui_goButton->setObjectName(QString::fromUtf8("ui_goButton"));
        ui_goButton->setMaximumSize(QSize(20, 16777215));
        ui_goButton->setIcon(icon3);
        ui_goButton->setDefault(false);
        ui_goButton->setFlat(true);

        horizontalLayout->addWidget(ui_goButton);


        verticalLayout->addLayout(horizontalLayout);

        ui_webView = new QWebView(WebTab);
        ui_webView->setObjectName(QString::fromUtf8("ui_webView"));
        ui_webView->setUrl(QUrl("about:blank"));

        verticalLayout->addWidget(ui_webView);


        retranslateUi(WebTab);
        QObject::connect(ui_goButton, SIGNAL(clicked()), actionGo, SLOT(trigger()));
        QObject::connect(ui_urlBar, SIGNAL(returnPressed()), actionGo, SLOT(trigger()));
        QObject::connect(actionReload, SIGNAL(triggered()), ui_webView, SLOT(reload()));
        QObject::connect(actionBack, SIGNAL(triggered()), ui_webView, SLOT(back()));
        QObject::connect(actionForward, SIGNAL(triggered()), ui_webView, SLOT(forward()));

        QMetaObject::connectSlotsByName(WebTab);
    } // setupUi

    void retranslateUi(QWidget *WebTab)
    {
        WebTab->setWindowTitle(QApplication::translate("WebTab", "Form", 0, QApplication::UnicodeUTF8));
        actionBack->setText(QApplication::translate("WebTab", "Back", 0, QApplication::UnicodeUTF8));
        actionForward->setText(QApplication::translate("WebTab", "Forward", 0, QApplication::UnicodeUTF8));
        actionReload->setText(QApplication::translate("WebTab", "Reload", 0, QApplication::UnicodeUTF8));
        actionGo->setText(QApplication::translate("WebTab", "Go", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("WebTab", "Copy", 0, QApplication::UnicodeUTF8));
        actionCopy->setShortcut(QApplication::translate("WebTab", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("WebTab", "Cut", 0, QApplication::UnicodeUTF8));
        actionCut->setShortcut(QApplication::translate("WebTab", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("WebTab", "Paste", 0, QApplication::UnicodeUTF8));
        actionPaste->setShortcut(QApplication::translate("WebTab", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        ui_goButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WebTab: public Ui_WebTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBTAB_H

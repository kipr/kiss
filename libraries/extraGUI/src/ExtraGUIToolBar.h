#ifndef EXTRAGUITOOLBAR_H
#define EXTRAGUITOOLBAR_H

#include <QtGui/QWidget>
#include <QDockWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciscintillabase.h>
#include <QAction>

#include "FunctionDialog.h"
#include "HelpWindow.h"
#include "SourceFile.h"

namespace Ui {
    class ExtraGUIToolBar;
}

class ExtraGUIToolBar : public QWidget  {
    Q_OBJECT
public:
    ExtraGUIToolBar(QWidget *parent = 0);
    ~ExtraGUIToolBar();

    int loopIndex();
    int modeIndex();
    void setInsertText(QString objectName);
    
    void setCurrentFile(SourceFile *sourceFile);
    
    QAction *toolbarAction() {return &m_action;}

protected:
    void changeEvent(QEvent *e);

private:

    Ui::ExtraGUIToolBar *m_ui;
    FunctionDialog *m_funcDialog;
    QMimeData *m_dragText;
    const QMimeData *m_dropData;
    QDrag *m_drag;
    QLabel *m_childAtPosition;
    QPoint m_dragStartPosition;
    QString m_insertText;
    int m_insertLine;
    HelpWindow *m_help;

    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

public slots:
    void buttonClicked();
    void helpRequested();
    void toolBarDropEvent(QDropEvent *event);
    void newFunctionFromToolBar(int typeOfCall);
    void newFromTemplate();
    void toggleWindow();

signals:
    void toolBarButtonClicked(QString insertText);
    void newFunction(int typeOfCall);
    void templateClicked();
    void callOutsideAction();

private:
  SourceFile *m_currentSourceFile;
  QAction m_action;

};

#endif // EXTRAGUITOOLBAR_H

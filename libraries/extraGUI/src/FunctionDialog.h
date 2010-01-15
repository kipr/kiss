#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include <QtGui/QDialog>
#include <QCursor>
#include <QKeyEvent>
#include <QModelIndex>
#include <QMessageBox>

namespace Ui {
    class FunctionDialog;
}

class FunctionDialog : public QDialog {
    Q_OBJECT
public:
    FunctionDialog(QWidget *parent = 0);
    ~FunctionDialog();
    QString* prototype();
    bool okWasPressed();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FunctionDialog *m_ui;

    QString *m_prototype;
    bool m_okWasPressed;
    int m_numParam;

private slots:
    void addParam();
    void removeParam();
    void okPressed();
    void cancelPressed();
};

#endif // FUNCTIONDIALOG_H

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QtGui/QWidget>
#include <QEvent>

namespace Ui {
    class HelpWindow;
}

class HelpWindow : public QWidget {
    Q_OBJECT
public:
    HelpWindow(QWidget *parent = 0);
    ~HelpWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::HelpWindow *m_ui;
};

#endif // HELPWINDOW_H

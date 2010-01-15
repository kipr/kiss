#include "HelpWindow.h"
#include "ui_HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::HelpWindow)
{
    m_ui->setupUi(this);
}

HelpWindow::~HelpWindow()
{
    delete m_ui;
}

void HelpWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

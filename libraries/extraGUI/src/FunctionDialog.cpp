#include "FunctionDialog.h"
#include "ui_FunctionDialog.h"

FunctionDialog::FunctionDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::FunctionDialog)
{
    m_ui->setupUi(this);

    m_numParam = -1;
    m_prototype = new QString();

    m_ui->funcType->setFocus();

    connect(m_ui->addParam, SIGNAL(clicked()), this, SLOT(addParam()));
    connect(m_ui->removeParam, SIGNAL(clicked()), this, SLOT(removeParam()));
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
    connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancelPressed()));

}

FunctionDialog::~FunctionDialog()
{
    delete m_ui;
}

void FunctionDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void FunctionDialog::addParam()
{
    QTableWidgetItem *type;
    type = new QTableWidgetItem();
    /*Get entered text for type*/
    type->setText(m_ui->enterParamType->text());
    QTableWidgetItem *name;
    name = new QTableWidgetItem();
    /*Get entered text for name*/
    name->setText(m_ui->enterParamName->text());
    /*Check to see if both blanks are filled in*/
    if(type->text().isEmpty())
    {
        QMessageBox::warning(this, "Missing Field", "Please enter the parameter's type.\n", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        m_ui->enterParamType->setFocus();
        return;
    }
    else if(name->text().isEmpty())
    {
        QMessageBox::warning(this, "Entry Error", "Please enter the parameter's name.\n", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        m_ui->enterParamName->setFocus();
        return;
    }
    else
    {
        /*Add type and name to table*/
        m_numParam++;
        m_ui->paramTable->setRowCount(m_numParam +1);
        m_ui->paramTable->setItem(m_numParam, 0, type);
        m_ui->paramTable->setItem(m_numParam, 1, name);
    }
    /*Clear line edits and prepare for new entries*/
    m_ui->enterParamType->clear();
    m_ui->enterParamName->clear();
    m_ui->enterParamType->setFocus();

}
void FunctionDialog::removeParam()
{
    int i;
    bool exit = false;

    while(exit == false)
    {
        if(m_numParam > -1)  //there are parameters to delete
        {
            for(i = m_numParam; i>=0; i--)
            {
                /*Check to see if an item is selected; if it is, delete that row*/
                if(m_ui->paramTable->item(i, 0)->isSelected() | m_ui->paramTable->item(i, 1)->isSelected())
                {
                    m_ui->paramTable->removeRow(i);

                    /*Update table to reflect loss of item*/
                    m_numParam--;
                    m_ui->paramTable->setRowCount(m_numParam + 1);
                }
            }
            exit = true;
        }

        else      //there are no parameters to delete
           exit = true;
    }

    m_ui->paramTable->clearSelection();
    m_ui->enterParamType->setFocus();
}
void FunctionDialog::okPressed()
{
    int i=0;

    /*Show warning message if there is no function type or name*/
    if(m_ui->funcType->text().isEmpty() && m_ui->funcName->text().isEmpty())
    {
        QMessageBox::warning(this, "Entry Error", "Please enter a function type and name.\n", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        m_okWasPressed = false;
        m_ui->funcType->setFocus();
    }
    else if(m_ui->funcType->text().isEmpty())
    {
        QMessageBox::warning(this, "Entry Error", "Please enter a function type.\n", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        m_okWasPressed = false;
        m_ui->funcType->setFocus();
    }

    else if(m_ui->funcName->text().isEmpty())
    {
        QMessageBox::warning(this, "Entry Error", "Please enter a function name.\n", QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
        m_okWasPressed = false;
        m_ui->funcName->setFocus();
    }

    else
        {
        m_prototype->clear();
        m_prototype->append(m_ui->funcType->text());
        m_prototype->append(" ");
        m_prototype->append(m_ui->funcName->text());
        m_prototype->append("(");

        if(m_numParam>-1)
        {
            for(i = 0; i<(m_numParam); i++)
            {
                m_prototype->append(m_ui->paramTable->item(i, 0)->text());
                m_prototype->append(" ");
                m_prototype->append(m_ui->paramTable->item(i, 1)->text());
                m_prototype->append(", ");

            }

            i=(m_numParam);
            m_prototype->append(m_ui->paramTable->item(i, 0)->text());
            m_prototype->append(" ");
            m_prototype->append(m_ui->paramTable->item(i,1)->text());
            m_prototype->append(")");
        }
        else
            m_prototype->append(")");

    m_okWasPressed = true;
    accept(); //close dialog box

    /*Make sure all entry fields are clear and reset m_numParam*/
    m_ui->funcType->clear();
    m_ui->funcName->clear();
    m_ui->enterParamType->clear();
    m_ui->enterParamName->clear();
    m_ui->paramTable->setRowCount(0);
    m_ui->funcType->setFocus();
    m_numParam = -1;
    }
}
void FunctionDialog::cancelPressed()
{
    /*Make sure all entry fields are clear and reset m_numParam*/
    m_ui->funcType->clear();
    m_ui->funcName->clear();
    m_ui->enterParamType->clear();
    m_ui->enterParamName->clear();
    m_ui->paramTable->setRowCount(0);
    m_ui->funcType->setFocus();
    m_numParam = -1;

    m_okWasPressed = false;
}
QString* FunctionDialog::prototype()
{
    return m_prototype;
}
bool FunctionDialog::okWasPressed()
{
    return m_okWasPressed;
}

#include "ExtraGUIToolBar.h"
#include "ui_ExtraGUIToolBar.h"

#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciscintillabase.h>

ExtraGUIToolBar::ExtraGUIToolBar(QWidget *parent) : QWidget(parent), m_ui(new Ui::ExtraGUIToolBar), m_currentSourceFile(0), m_action("Extra GUI", this)
{
    Q_INIT_RESOURCE(extra_gui);
    m_ui->setupUi(this);

    m_funcDialog = new FunctionDialog();
    m_help = new HelpWindow();

    /*Connections*/
    //connect(m_ui->newFromTemplate, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_ui->conditionalIf, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_ui->ifElse, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_ui->conditionalSwitch, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_ui->loop, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_ui->function, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_ui->help, SIGNAL(clicked()), this, SLOT(helpRequested()));
    connect(this, SIGNAL(newFunction(int)), this, SLOT(newFunctionFromToolBar(int)));
    connect(this, SIGNAL(templateClicked()), this, SLOT(newFromTemplate()));
    connect(&m_action, SIGNAL(triggered()), this, SLOT(toggleWindow()));

}

ExtraGUIToolBar::~ExtraGUIToolBar()
{
    delete m_ui;
    Q_CLEANUP_RESOURCE(extra_gui);
}

void ExtraGUIToolBar::toggleWindow()
{
    if(isVisible()) close();
    else show();
}

void ExtraGUIToolBar::changeEvent(QEvent *e)
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

int ExtraGUIToolBar::loopIndex()
{
    return m_ui->loopComboBox->currentIndex();
}

int ExtraGUIToolBar::modeIndex()
{
    return 0;
    //return m_ui->modeComboBox->currentIndex();
}

void ExtraGUIToolBar::setInsertText(QString objectName)
{
    /*Clear m_insertText*/
    m_insertText.clear();

    /*Set text to insert based on which button or label has been selected*/
    if(objectName == "conditionalIf" || objectName == "ifLabel")
    {
        m_insertText.append("    if(boolean_expression)\n");
        m_insertText.append("    { \n        //What will happen if this is true? \n    }\n");
    }
    else if(objectName == "ifElse" || objectName == "ifElseLabel")
    {
        m_insertText.append("    if(boolean_expression)\n");
        m_insertText.append("    {\n        //What will happen if this is true? \n    }\n");
        m_insertText.append("    else\n");
        m_insertText.append("    {\n        //What will happen if this is false?\n    }\n");
    }
    else if(objectName == "conditionalSwitch" || objectName == "switchLabel")
    {
        m_insertText.append("    switch(variable)\n    {\n");
        m_insertText.append("        case a_value:\n");
        m_insertText.append("        {\n            //What happens if variable == a_value?\n");
        m_insertText.append("            break;\n        }    \n");
        m_insertText.append("        //other cases go here\n    }\n");
    }
    else if(objectName == "loop" || objectName == "loopLabel")
    {
        switch(loopIndex())
        {
            case 0:
            {
                m_insertText.append("    for(variable = starting_value; variable <, <=, >=, or > ending_value; change_in_variable)\n");
                m_insertText.append("    {\n        //What needs to be looped?\n    }\n");
                break;
            }
            case 1:
            {
                m_insertText.append("    while(boolean_expression)\n");
                m_insertText.append("    {\n        //What code will be looped while this is true?\n    }\n");
                break;
            }
        }
    }
    else if(objectName == "functionLabel")
        m_insertText.append("function");

}

void ExtraGUIToolBar::mousePressEvent(QMouseEvent *event)
{
    /*Ignore if there is no child at this spot*/
    if(childAt(event->pos()) == 0)
           event->ignore();
    /*Ignore if click is on the line or on the mode label*/
    else if(childAt(event->pos()) == m_ui->line)
            event->ignore();
    /*Ignore right clicks*/
    else if(event->button() == Qt::RightButton)
           event->ignore();

    /*Begin drag if child is draggable*/
    else
    {
        m_childAtPosition = qobject_cast<QLabel *>(childAt(event->pos()));
        m_dragStartPosition = event->pos();
        if(m_childAtPosition->objectName() != "templateLabel")
        {
            setInsertText(m_childAtPosition->objectName());
            m_dragText = new QMimeData;
            m_dragText->setText(m_insertText);

            m_drag = new QDrag(this);
            m_drag->setPixmap(*(m_childAtPosition->pixmap()));
            m_drag->setMimeData(m_dragText);
            m_drag->start();
        }
    }
}

void ExtraGUIToolBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    /*Ignore double clicks*/
    event->ignore();

}

void ExtraGUIToolBar::buttonClicked()
{

    /*Find which button has been clicked and emit the correct signal*/
    QObject *senderObject = sender();
    QPushButton *senderButton = qobject_cast<QPushButton *>(senderObject);

    if(senderObject->objectName() == "newFromTemplate")
        emit templateClicked();

    else if(senderObject->objectName() == "function")
        emit newFunction(0);

    else //Regular text insertion button
    {
        setInsertText(senderButton->objectName());
        if(!m_currentSourceFile)
            return;
        else
            m_currentSourceFile->insert(m_insertText);
    }
}

void ExtraGUIToolBar::helpRequested()
{
    m_help->show();
}

void ExtraGUIToolBar::toolBarDropEvent(QDropEvent *event)
{
   m_dropData = event->mimeData();
   int pos_y;
   int topLineShown = 0;

   /*Check which line is currently at the top of the screen*/
   topLineShown = m_currentSourceFile->firstVisibleLine();

   /*Get Y position of drop event and convert to a line number.
     (This is to avoid problems when the drop is desired at a place currently without
        a line of text.) */
   pos_y = event->pos().y();
   m_insertLine = topLineShown + (pos_y - 15)/16;

   if(m_dropData->text() == "function")
       emit newFunction(1);

   else
       m_currentSourceFile->insertAt(m_dropData->text(), m_insertLine, 0);
}

void ExtraGUIToolBar::newFunctionFromToolBar(int typeOfCall)
{
    int line = 0;
    int index = 0;
    int newLine = 0;
    int newIndex = 0;
    int mainLine = 0;
    int functionLine = 0;
    int functionIndex = 0;
    bool textFound = true;

    m_funcDialog->exec();

    if(!m_currentSourceFile)
        return;
    else
    {
        if(m_funcDialog->okWasPressed())
        {

            if(typeOfCall == 1) //the function is being created from a drag and drop, not a click
                m_currentSourceFile->setCursorPosition(m_insertLine, 0);

            /*Get functionLine from current cursor position*/
            m_currentSourceFile->getCursorPosition(&functionLine, &functionIndex);

            /*Begin code to find insertion point for prototype*/
            /*Find location of "Function prototypes below" text*/
            if(m_currentSourceFile->findFirst("Function prototypes below", true, false, true, true, true, 0, 0, false))
                m_currentSourceFile->getCursorPosition(&line, &index);

            /*Decide where to add this text and the prototype if the text cannot already be found*/
            else
            {
                /*Find location of main function if it exists*/
                if(m_currentSourceFile->findFirst("main", true, false, true, true, true, 0, 0, false))
                    m_currentSourceFile->getCursorPosition(&mainLine, &index);
                else
                    mainLine = m_currentSourceFile->lines();

                /*Find location of first preprocessor command and set cursor to this position*/
                if(m_currentSourceFile->findFirst("#", true, false, true, true, true, 0, 0, false))
                {
                    m_currentSourceFile->getCursorPosition(&line, &index);

                    while(textFound == true)
                    {
                        m_currentSourceFile->findNext();
                        /*Get location of cursor after next preprocessor command is found*/
                        m_currentSourceFile->getCursorPosition(&newLine, &newIndex);

                        /*Ignore anything found beyond the beginning of the main function*/
                        if(newLine >= mainLine)
                            textFound = false;
                        else
                        {
                            /*If next # is not farther down or to the right in the document, ignore it*/
                            if((newLine > line)||(newIndex > index))
                                line = newLine;
                            else
                                textFound = false;
                        }
                    }
                    line = line + 2;
                }
                m_currentSourceFile->insertAt("/*Function prototypes below*/\n\n", line, 0);
            }

            /*Insert function prototype*/
            line++;
            m_currentSourceFile->setCursorPosition(line, 0);
            m_currentSourceFile->insert(";\n");
            m_currentSourceFile->insert(*(m_funcDialog->prototype()));

             /*Insert function definition skeleton*/

            if(functionLine <= line) //Check if the function definition will precede the prototype
            {
                functionLine = line;
                /*Find the first open line after the prototypes to place the function instead*/
                while(m_currentSourceFile->text(functionLine) != "\n")
                {
                    functionLine++;
                }
            }
            m_currentSourceFile->setCursorPosition(functionLine+1, 0);

            m_currentSourceFile->insert("\n{\n    //Function definition goes here.\n}\n");
            m_currentSourceFile->insert(*(m_funcDialog->prototype()));

        }
    }
}

void ExtraGUIToolBar::newFromTemplate()
{
    QString templateText;

    emit(callOutsideAction());

    switch(modeIndex())
    {
        case 0:  //Standard mode
        {

            m_currentSourceFile->insertAt("\n\n\n\n\n", 0, 0);
            m_currentSourceFile->insertAt("/***************************************************************************/", 0, 0);
            m_currentSourceFile->insertAt("/*********     This comment should be filled with a description    *********/", 1, 0);
            m_currentSourceFile->insertAt("/*********         of your program and what it does.               *********/", 2, 0);
            m_currentSourceFile->insertAt("/***************************************************************************/", 3, 0);

            templateText.append("/* #defines and constants go here.*/\n\n");
            templateText.append("/*Global variables go here (if you absolutely need them).*/\n\n");
            templateText.append("/*Function prototypes below*/\n\n");
            templateText.append("int main()\n{\n");
            templateText.append("    //All variable declarations go here.\n\n");
            templateText.append("    //This is your main function.\n\n\n");
            templateText.append("}\n\n");
            templateText.append("/*Declare functions here as needed.*/");

            m_currentSourceFile->insert(templateText);

            break;
        }

        case 1:  //Botball mode
        {
            m_currentSourceFile->insertAt("\n\n\n\n\n\n\n", 0, 0);
            m_currentSourceFile->insertAt("/***************************************************************************/", 0, 0);
            m_currentSourceFile->insertAt("/*********     This comment should be filled with a description    *********/", 1, 0);
            m_currentSourceFile->insertAt("/*********         of your program and what it does.               *********/", 2, 0);
            m_currentSourceFile->insertAt("/*********       This template is for Botball programs.            *********/", 3, 0);
            m_currentSourceFile->insertAt("/*********    Make sure you have selected the CBC target.          *********/", 4, 0);
            m_currentSourceFile->insertAt("/***************************************************************************/", 5, 0);

            templateText.append("/* Libraries used in Botball are automatically included, but list any additional includes here */\n\n");
            templateText.append("/* #defines and constants go here.*/\n\n");
            templateText.append("/*Global variables go here (if you absolutely need them).*/\n\n");
            templateText.append("/*Function prototypes below*/\n\n");
            templateText.append("int main()\n{\n");
            templateText.append("    //All variable declarations go here.\n\n");
            templateText.append("    //This is your main function.\n\n");
            templateText.append("    //Wait for starting light with light sensor in the_port_number\n");
            templateText.append("        wait_for_light(the_port_number);\n\n");
            templateText.append("    //Run your_code for the amount_of_time\n");
            templateText.append("        run_for(amount_of_time, your_code);\n\n");
            templateText.append("    //Make sure your motors are stopped\n");
            templateText.append("        ao();\n");
            templateText.append("        create_stop();\n\n");
            templateText.append("    //Put anything below you want your robot to do after the game (e.g. play victory song).\n\n\n");
            templateText.append("}\n\n");
            templateText.append("/*Declare functions here as needed.*/");

            m_currentSourceFile->insert(templateText);

            break;
        }
    }
}

void ExtraGUIToolBar::setCurrentFile(SourceFile *sourceFile)
{
    if(m_currentSourceFile)
        disconnect(m_currentSourceFile, SIGNAL(handleDrop(QDropEvent*)), this, SLOT(toolBarDropEvent(QDropEvent*)));
    
    m_currentSourceFile = sourceFile;
    
    if(m_currentSourceFile)
        connect(m_currentSourceFile, SIGNAL(handleDrop(QDropEvent*)), this, SLOT(toolBarDropEvent(QDropEvent*)));
}

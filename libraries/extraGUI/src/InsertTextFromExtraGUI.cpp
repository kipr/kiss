/*ADDED BY NB*/
void MainWindow::insertTextFromToolBar(QString insertText)
{
    if(!m_currentSourceFile)
        return;
    else
        m_currentSourceFile->insert(insertText);
}

void MainWindow::newFunctionFromToolBar(int typeOfCall)
{
    int line = 0;
    int index = 0;
    int newLine = 0;
    int newIndex = 0;
    int mainLine = 0;
    int functionLine = 0;
    int functionIndex = 0;
    bool textFound = true;

    m_extraTB->funcDialog()->exec();

    if(!m_currentSourceFile)
        return;
    else
    {
        if(m_extraTB->funcDialog()->okWasPressed())
        {

            if(typeOfCall == 1) //the function is being created from a drag and drop, not a click
                m_currentSourceFile->setCursorPosition(m_currentSourceFile->insertLine(), 0);

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
            m_currentSourceFile->insert(*(m_extraTB->funcDialog()->prototype()));

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
            m_currentSourceFile->insert(*(m_extraTB->funcDialog()->prototype()));

        }
    }
}

void MainWindow::newFromTemplate()
{
    QString templateText;

    on_actionNew_triggered(true);

    switch(m_extraTB->modeIndex())
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

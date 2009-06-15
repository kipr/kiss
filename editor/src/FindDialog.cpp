/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "FindDialog.h"
#include <QMessageBox>

// Constructor & Destructor
FindDialog::FindDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	setModified(false);
}

FindDialog::~FindDialog()
{
}

// Sets the current SourceFile object
void FindDialog::setSourceFile(SourceFile *sourceFile)
{
	m_currentSourceFile = sourceFile;
	setModified(true);
}

// Finds the first occurence of the specified text in the current source file
void FindDialog::on_ui_buttonFind_clicked(bool)
{
	bool found;
	if(isModified()) {
		found = m_currentSourceFile->findFirst(ui_findInput->text(), false, ui_matchCaseCheckBox->isChecked(), false, true);
		setModified(false);
	}
	else
		found = m_currentSourceFile->findNext();
	if(!found)
		QMessageBox::warning(this, "Not found", "Not Found");
	else
		close();
}

// Replaces the currently found text (if any) and then finds again
void FindDialog::on_ui_buttonReplace_clicked(bool)
{
	m_currentSourceFile->replace(ui_replaceInput->text());
	on_ui_buttonFind_clicked();
}

// Replaces all instances of the specified find text and replaces them with the specified replace text
void FindDialog::on_ui_buttonReplace_All_clicked(bool)
{
	m_currentSourceFile->setCursorPosition(0, 0);

	if(!m_currentSourceFile->findFirst(ui_findInput->text(), false, ui_matchCaseCheckBox->isChecked(), false, false))
		return;
	
	do
		m_currentSourceFile->replace(ui_replaceInput->text());
	while(m_currentSourceFile->findNext());
}

bool FindDialog::isModified()
{
	return m_isModified;
}

void FindDialog::setModified(bool modified)
{
	m_isModified = modified;
}

void FindDialog::on_ui_findInput_textChanged(QString)
{
	setModified(true);
}

void FindDialog::on_ui_matchCaseCheckBox_stateChanged(int)
{
	setModified(true);
}


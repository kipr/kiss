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

#ifndef __FINDDIALOG_H__
#define __FINDDIALOG_H__

#include "ui_FindDialog.h"
#include "SourceFile.h"

#include <QDialog>

/* FindDialog Class inherits QDialog and Ui::FindDialog
 *
 * List of Ui::FindDialog inherited widgets:
 *	QLineEdit	ui_findInput
 *	QLineEdit	ui_replaceInput
 *	QPushButton ui_buttonFind
 *	QPushButton ui_buttonReplace
 *  QPushButton ui_buttonReplace_All
 *	QPushButton ui_buttonClose
 *	QCheckBox	ui_matchCaseCheckBox
 *
 */

class FindDialog : public QDialog, private Ui::FindDialog
{
Q_OBJECT

public:
	// Constructor & Destructor
	FindDialog(QWidget *parent = 0);
	~FindDialog();

	// Set the source file to run find on
	void setSourceFile(SourceFile *sourceFile);
	
	bool isModified();

private:
	SourceFile *m_currentSourceFile;
	bool m_isModified;

public slots:
	// Auto slots to handle the find, replace, and replace all buttons
	void on_ui_buttonFind_clicked(bool checked = false);
	void on_ui_buttonReplace_clicked(bool checked = false);
	void on_ui_buttonReplace_All_clicked(bool checked = false);
	
	void on_ui_findInput_textChanged(QString text);
	void on_ui_matchCaseCheckBox_stateChanged(int state);
	
	void setModified(bool modified = true);

};

#endif


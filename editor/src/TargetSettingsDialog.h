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

#ifndef __TARGETSETTINGSDIALOG_H__
#define __TARGETSETTINGSDIALOG_H__

#include "ui_TargetSettingsDialog.h"
#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>

/* TargetSettingsDialog Class inherits QDialog and Ui::TargetSettingsDialog
 *
 * List of Ui::TargetSettingsDialog inherited widgets:
 *	QDialogButtonBox	ui_buttonBox
 *	QPushButton			ui_deleteButton
 *	QPushButton			ui_insertButton
 *  QTableWidget		ui_settingsTable
 *
 */

class TargetSettingsDialog : public QDialog, private Ui::TargetSettingsDialog
{
Q_OBJECT

public:
	// Constructor & Destructor
	TargetSettingsDialog(QWidget *parent = 0);
	~TargetSettingsDialog();
	
	// Sets the target file that will be edited when exec is called
	void setTargetFile(QString filename);
	
	// Run the dialog
	int exec();

public slots:
	// Auto slots that deal with the insert and delete buttons
	void on_ui_insertButton_clicked(bool checked = false);
	void on_ui_deleteButton_clicked(bool checked = false);
	
private:
	QString m_targetFile;
	
	// Two functions to deal with 
	void loadTable();
	void saveTable();
};

#endif


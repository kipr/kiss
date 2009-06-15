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

#ifndef __CHOOSEPORTDIALOG_H__
#define __CHOOSEPORTDIALOG_H__

#include "ui_ChoosePortDialog.h"
#include <QDialog>
#include <QRadioButton>
#include <QList>

/* ChoosePortDialog Class inherits QDialog and Ui::ChoosePortDialog
 *
 * List of Ui::ChoosePortDialog inherited widgets:
 *	QPushButton	ui_cancelButton
 *  QPushButton	ui_okButton
 *	QPushButton ui_refreshButton
 *	QGroupBox	ui_portGroupBox
 *
 */

class ChoosePortDialog : public QDialog, private Ui::ChoosePortDialog
{
Q_OBJECT

public:
	// Constructor & Destructor
	ChoosePortDialog(QWidget *parent = 0);
	~ChoosePortDialog();

	// Runs the dialog
	int exec();
	
	// Returns a string containing the selected port name
	QString getSelectedPortName();
	
public slots:
	// Refreshes the list of ports
	void refreshPortList();
	
	void on_ui_addPortButton_clicked(bool checked = false);

private:
	QList<QRadioButton*> m_portButtonList;

	// Clears and deletes all entries in the port list
	void clearPortButtonList();

};

#endif

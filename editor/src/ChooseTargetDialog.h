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

#ifndef __CHOOSETARGETDIALOG_H__
#define __CHOOSETARGETDIALOG_H__

#include "ui_ChooseTargetDialog.h"

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QSettings>
#include <vector>

/* ChooseTargetDialog Class inherits QDialog and Ui::ChooseTargetDialog
 *
 * List of Ui::TargetSettingsDialog inherited widgets:
 *	QDialogButtonBox	ui_buttonBox
 *	QListWidget			ui_listWidget
 *
 */

class ChooseTargetDialog : public QDialog, private Ui::ChooseTargetDialog
{
Q_OBJECT

public:
	//Constructor & Destructor
	ChooseTargetDialog(QWidget *parent = 0);
	~ChooseTargetDialog();

	//Show the dialog
	int exec();

	// Returns to the path of the target file for the selected target
	QString getSelectedTargetFilePath();

private:
	QList<QString> m_targetFileList;

	// Scan kiss-c/targets for available target directories (with .target files in them)
	bool scanTargetDirectory();
};

#endif


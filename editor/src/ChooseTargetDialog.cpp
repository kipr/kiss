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

#include "ChooseTargetDialog.h"
#include <QDir>
#include <QMessageBox>
#include <QListWidget>

ChooseTargetDialog::ChooseTargetDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	// Turn off editing in the list widget, and only allow one selection at a time
	ui_listWidget->setEditTriggers(QListView::NoEditTriggers);
	ui_listWidget->setSelectionMode(QListView::SingleSelection);
}

ChooseTargetDialog::~ChooseTargetDialog()
{
}

bool ChooseTargetDialog::scanTargetDirectory()
{
	QDir targetDir(QDir::currentPath() + "/targets");
	QStringList targetDirs;
	QStringList targetList;

	// Clean up any leftovers from the last time this was run
	m_targetFileList.clear();
	ui_listWidget->clear();

	// Choke if we can't find the target directory
	if(!targetDir.exists()) {
		qWarning("ChooseTargetDialog::scanTargetDirectory: Could not find targets directory");
		ui_listWidget->clear();
		return false;
	}
	
	// Get a list of the possible target directories and check through them
	targetDirs = targetDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
	QStringListIterator i(targetDirs);

	while(i.hasNext()) {
		QString dirName = i.next();
		targetDir.cd(dirName);
		
		// The target file naming scheme is <dirname>.target
		QFileInfo targetFile(targetDir, dirName + ".target");

		// If we can't find a target file, skip this directory
		if(!targetFile.exists()) {
			qWarning("ChooseTargetDialog::scanTargetDirectory: Invalid target directory %s", qPrintable(targetDir.absolutePath()));
			targetDir.cdUp();
			continue;
		}

		// Add the target file to the list
		m_targetFileList.push_back(targetFile.absoluteFilePath());
		
		QSettings target(m_targetFileList.last(), QSettings::IniFormat);
		targetList << target.value("display_name").toString();
		targetDir.cdUp();
	}

	// Didn't find any targets, quit
	if(targetList.count() == 0) {
		qWarning("ChooseTargetDialog::scanTargetDirectory: No targets to display!\n");
		return false;
	}

	// Add the targets we found, and focus on the top one
	ui_listWidget->addItems(targetList);
	ui_listWidget->setCurrentItem(ui_listWidget->item(0));
	return true;
}

// Show the dialog, scanning the target dir first
int ChooseTargetDialog::exec()
{
	if(!scanTargetDirectory()) {
		setVisible(false);
		QMessageBox::critical(parentWidget(), "Error", "Failed to scan targets directory.");
		return QDialog::Rejected;
	}
	else
		return QDialog::exec();
}

// Return the path to the current target file
QString ChooseTargetDialog::getSelectedTargetFilePath()
{
	return m_targetFileList.at(ui_listWidget->row(ui_listWidget->currentItem()));
}


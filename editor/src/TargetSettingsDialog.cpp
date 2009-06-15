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

#include "TargetSettingsDialog.h"
#include <QSettings>
#include <QStringList>
#include <QInputDialog>

TargetSettingsDialog::TargetSettingsDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	
	// Sets up the table for two columns (key/value)
	ui_settingsTable->setColumnCount(2);
}

TargetSettingsDialog::~TargetSettingsDialog()
{
}


// Sets the target file :)
void TargetSettingsDialog::setTargetFile(QString filename)
{
	m_targetFile = filename;
}

// Loads the target file data, execs the dialog
//     and saves it again if Accept is clicked
int TargetSettingsDialog::exec()
{
	loadTable();
	if(QDialog::exec() == QDialog::Rejected)
		return QDialog::Rejected;
	saveTable();
	return QDialog::Accepted;
}

// Simply loads the target file into the ui_settingsTable
void TargetSettingsDialog::loadTable()
{
	// Clear the table first, and re-set the labels
	ui_settingsTable->clear();
	ui_settingsTable->setRowCount(0);
	ui_settingsTable->setHorizontalHeaderLabels(QStringList() << "Key" << "Value");
	
	// If the target file is blank, leave it cleared
	if(m_targetFile == "")
		return;

	// Opens up the target file (just a windows style ini file)
	QSettings settings(m_targetFile, QSettings::IniFormat);
	settings.beginGroup("Target");
	
	QStringList keys = settings.allKeys();
	QStringListIterator i(keys);
	
	// Make sure there's enough space in the table
	ui_settingsTable->setRowCount(keys.count());
	
	for(int j = 0;i.hasNext();j++) {
		QString key = i.next();

		// Add the key and turn off editing
		QTableWidgetItem *item = new QTableWidgetItem(key);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		ui_settingsTable->setItem(j, 0, item);

		// Add the item itself
		item = new QTableWidgetItem(settings.value(key).toString());
		ui_settingsTable->setItem(j, 1, item);
	}

	// Make sure that the value column is wide enough
	ui_settingsTable->resizeColumnToContents(1);
}

// Save the data currently in the ui_settingsTable to the target file
void TargetSettingsDialog::saveTable()
{
	// Open the settings file
	QSettings settings(m_targetFile, QSettings::IniFormat);
	settings.beginGroup("Target");
	
	settings.remove("");

	// Write out all the key/value pairs
	for(int i = 0;i < ui_settingsTable->rowCount();i++)
		settings.setValue(ui_settingsTable->item(i,0)->text(), ui_settingsTable->item(i,1)->text());

	settings.endGroup();
}

// Insert a new key
void TargetSettingsDialog::on_ui_insertButton_clicked(bool)
{
	// Get the key name
	QString key = QInputDialog::getText(this, "Insert Key", "Key:");

	// Don't do anything if the user clicked cancel or didn't enter a name
	if(!key.length())
		return;

	// Make sure there are enough rows
	ui_settingsTable->setRowCount(ui_settingsTable->rowCount() + 1);

	// Add the key, making it uneditable
	QTableWidgetItem *item = new QTableWidgetItem(key);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	ui_settingsTable->setItem(ui_settingsTable->rowCount()-1, 0, item);

	// Add the corresponding item
	ui_settingsTable->setItem(ui_settingsTable->rowCount()-1, 1, new QTableWidgetItem());
}

// Deletes a row from the table
void TargetSettingsDialog::on_ui_deleteButton_clicked(bool)
{
	ui_settingsTable->removeRow(ui_settingsTable->currentRow());
}


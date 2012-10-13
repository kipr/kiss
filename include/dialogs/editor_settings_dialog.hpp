/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
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

#ifndef __EDITORSETTINGSDIALOG_H__
#define __EDITORSETTINGSDIALOG_H__

#include "ui_EditorSettingsDialog.h"

#include <QDialog>
#include <QObject>

#define EDITOR "Editor"
#define ENABLED "enabled"
#define AUTO_COMPLETION "autocompletion"
#define API_SOURCE "apisource"
#define DOC_SOURCE "docsource"
#define THRESHOLD "threshold"
#define STYLE "style"
#define MAINTAIN "Maintain"
#define INTELLIGENT "Intelligent"
#define CALL_TIPS "calltips"
#define LINE_NUMBERS "linenumbers"
#define WIDTH "width"
#define AUTO_INDENT "autoindent"
#define BRACE_MATCHING "bracematching"
#define DEBUGGER_ENABLED "debugger_enabled"

class EditorSettingsDialog : public QDialog, private Ui::EditorSettingsDialog
{
Q_OBJECT

public:
	// Constructor & Destructor
	EditorSettingsDialog(QWidget* parent = 0);
	~EditorSettingsDialog();

	// Show the dialog
	int exec();
	
private slots:
	void on_ui_buttonBox_clicked(QAbstractButton *button);

private:
	QMap<QString, QColor> m_lexerSettings;
	QColor m_backgroundColor;
	QFont m_font;
	int m_fontSize;
	
	// Save/Read the current settings
	void saveSettings();
	void readSettings();
};

#endif

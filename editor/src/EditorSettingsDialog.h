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
 **************************************************************************
 *
 * WARNING: The following information may be out of date
 *
 * EditorSettingsDialog Class inherits QDialog and Ui::EditorSettingsDialog
 *
 * List of Ui::EditorSettingsDialog inherited widgets:
 *	QGroupBox		ui_autoCompletionGroup
 *		QSpinBox	ui_autoCompletionThresholdSpinBox
 *		QCheckBox	ui_autoCompletionAPISourceCheckBox
 *		QCheckBox	ui_autoCompletionDocumentSourceCheckBox
 *		QCheckBox	ui_autoCompletionEnabledCheckBox
 *	QGroupBox			ui_autoIndentGroup
 *		QSpinBox		ui_autoIndentWidthSpinBox
 *		QRadioButton	ui_autoIndentIntelligentStyleRadioButton
 *		QRadioButton	ui_autoIndentMaintainStyleRadioButton
 *		QCheckBox		ui_autoIndentEnabledCheckBox
 *	QGroupBox			ui_fontGroup
 *		QFontComboBox	ui_fontBox
 *		QSpinBox		ui_fontSizeSpinBox
 *	QGroupBox		ui_miscGroup
 *		QCheckBox	ui_braceMatchingCheckBox
 *		QCheckBox	ui_callTipsCheckBox
 *		QCheckBox	ui_marginLineNumbersCheckBox
 *	QDialogButtonBox	ui_buttonBox
 *
 * Possible settings for the application stored in the Editor group:
 *
 *		[Editor]
 *			font=<Font Family>
 *			fontSize=<Point Size>
 *
 *			autocompletion\enabled=<true|false>
 *			autocompletion\threshold=4
 *			autocompletion\apisource=<true|false>
 *			autocompletion\docsource=<true|false>
 *
 *			autoindent\enabled=<true|false>
 *			autoindent\style=<Maintain|Intelligent>
 *			autoindent\width=<Int>
 *
 *			calltips=<true|false>
 *			bracematching=<true|false>
 *			marginlinenumbers=<true|false>
 *
 */

#ifndef __EDITORSETTINGSDIALOG_H__
#define __EDITORSETTINGSDIALOG_H__

#include "ui_EditorSettingsDialog.h"
#include <QDialog>
#include <QObject>

class EditorSettingsDialog : public QDialog, private Ui::EditorSettingsDialog
{
Q_OBJECT

public:
	// Constructor & Destructor
	EditorSettingsDialog(QWidget *parent = 0);
	~EditorSettingsDialog();

	// Show the dialog
	int exec();

private:
	// Save/Read the current settings
	void saveSettings();
	void readSettings();
};

#endif

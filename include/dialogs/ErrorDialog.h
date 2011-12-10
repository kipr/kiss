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

#ifndef _ERRORDIALOG_H_
#define _ERRORDIALOG_H_

#include "ui_ErrorDialog.h"

class ErrorDialog : public QDialog, public Ui::ErrorDialog
{
	Q_OBJECT
public:
	ErrorDialog(QWidget* parent = 0);
	
	void setMessage(const QString& error, const QStringList& args = QStringList());
	
	static void showError(QWidget* parent, const QString& error, const QStringList& args = QStringList());
private slots:
	void on_ui_error_anchorClicked(const QUrl& link);
	void on_ui_copy_clicked();
};

#endif

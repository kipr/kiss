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

#include "ErrorDialog.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QFile>

ErrorDialog::ErrorDialog(QWidget* widget) : QDialog(widget) { setupUi(this); }

void ErrorDialog::setMessage(const QString& error, const QStringList& args)
{
	ui_error->clear();
	
	QString message = QString("Error template ") + error + " does not exist.";
	QFile file(QString(":/errors/") + error + ".txt");
	if(file.open(QIODevice::ReadOnly)) {
		message = file.readAll();
	}
	for(int i = 0; i < args.size(); ++i) {
		message = message.replace(QString("${") + QString::number(i + 1) + "}", args.at(i));
	}
	ui_error->setText(message);
}

void ErrorDialog::on_ui_copy_clicked()
{
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(ui_error->toPlainText());
}

void ErrorDialog::on_ui_error_anchorClicked(const QUrl& link) { QDesktopServices::openUrl(link); }

void ErrorDialog::showError(QWidget* parent, const QString& error, const QStringList& args)
{
	ErrorDialog dialog(parent);
	dialog.setMessage(error, args);
	dialog.exec();
}
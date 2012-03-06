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

#include "MessageDialog.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QFile>

MessageDialog::MessageDialog(QWidget* widget) : QDialog(widget) { setupUi(this); }

void MessageDialog::setMessage(const QString& message, const QStringList& args)
{
	ui_message->clear();
	
	QString text = QString("Message template ") + message + " does not exist.";
	QFile file(QString(":/messages/") + message + ".txt");
	if(file.open(QIODevice::ReadOnly)) {
		text = file.readAll();
	}
	for(int i = 0; i < args.size(); ++i) {
		text = text.replace(QString("${") + QString::number(i + 1) + "}", args.at(i));
	}
	ui_message->setHtml(text);
}

void MessageDialog::setLabel(const QString& label)
{
	ui_label->setText(label);
}

void MessageDialog::on_ui_copy_clicked()
{
	QApplication::clipboard()->setText(ui_message->toPlainText());
}

void MessageDialog::on_ui_message_anchorClicked(const QUrl& link) { QDesktopServices::openUrl(link); }

void MessageDialog::showMessage(QWidget* parent, const QString& label, const QString& message, const QStringList& args)
{
	MessageDialog dialog(parent);
	dialog.setLabel(label);
	dialog.setMessage(message, args);
	dialog.exec();
}

void MessageDialog::showError(QWidget* parent, const QString& error, const QStringList& args)
{
	MessageDialog dialog(parent);
	dialog.setMessage(error, args);
	dialog.exec();
}
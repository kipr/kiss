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

#include "ChoosePortDialog.h"

#include <QStringList>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QList>
#include <QRadioButton>
#include <QInputDialog>

#ifndef Q_OS_WIN32
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

// Constructor
ChoosePortDialog::ChoosePortDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	// Set up the refresh button
	connect(ui_refreshButton, SIGNAL(clicked()), this, SLOT(refreshPortList()));
}

// Destructor
ChoosePortDialog::~ChoosePortDialog()
{
	clearPortButtonList();
}

// Clears the portButtonList, deleting the entries
//     and cleaning out the ui_portGroupBox
void ChoosePortDialog::clearPortButtonList()
{
	delete ui_portGroupBox->layout();
	
	QListIterator<QRadioButton*> i(m_portButtonList);
	while(i.hasNext())
		delete i.next();
	m_portButtonList.clear();
}
	
// Ugly code to check port availability
void ChoosePortDialog::refreshPortList()
{
	QStringList portList;

	clearPortButtonList();

#ifdef Q_OS_WIN32
	/* WINDOWS ONLY */

	// Sets the timeouts so that operations are non-blocking
	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout = MAXDWORD;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;


	// Scan through ports 1-32	
	for(int i = 1;i <= 32;i++) {
		QString portName = "com" + QString::number(i);
		QString realPortName = "\\\\.\\com" + QString::number(i);

		HANDLE handle = CreateFileA(realPortName.toLocal8Bit(), 
									GENERIC_READ | GENERIC_WRITE, 
									0, NULL, OPEN_EXISTING, 0, 0);

		// Add the port to the list of available names if it's valid
		if(handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
			portList << portName;
		}
	}
	/* END WINDOWS ONLY */
#else
	/* LINUX/MAC ONLY */
	QDir devDirectory("/dev");
	QStringList devListFilter;

// Determines how to look for serial port files
//     depending on your platform
#ifdef Q_OS_MAC
	devListFilter << "tty.*";
#else
	devListFilter << "ttyUSB*";
#endif

	// Basically the same as "ls /dev/tty.*" or "ls /dev/ttyS*"
	QFileInfoList potentialPortList = devDirectory.entryInfoList(devListFilter, QDir::System | QDir::CaseSensitive);

	//Check through the returned ports to see which ones might be valid
	QListIterator<QFileInfo> i(potentialPortList);
	while(i.hasNext()) {
		QFileInfo current = i.next();
		int stream = ::open(current.absoluteFilePath().toLocal8Bit(), O_RDWR | O_NDELAY);
		struct termios tio;
		// If I can get serial attributes then the port is good
		if(tcgetattr(stream, &tio) != -1)
			portList << current.absoluteFilePath();
		::close(stream);
	}
	/* END LINUX/MAC ONLY */
#endif

	// Add the ports we just found to ui_portGroupBox
	ui_portGroupBox->setLayout(new QVBoxLayout());
	QStringListIterator strings(portList);
	for(int i = 0;strings.hasNext();i++) {
		m_portButtonList.push_back(new QRadioButton(strings.next()));
		ui_portGroupBox->layout()->addWidget(m_portButtonList[i]);
	}
}

//Show the dialog, refresh the port list, and run the dialog
int ChoosePortDialog::exec()
{
	show();
	refreshPortList();
	return QDialog::exec();
}

// Figure out which port was selected and return it, or nothing if none are
QString ChoosePortDialog::getSelectedPortName()
{
	QListIterator<QRadioButton*> i(m_portButtonList);
	while(i.hasNext()) {
		QRadioButton *button = i.next();
		if(button->isChecked())
			return button->text();
	}
	return QString();
}

void ChoosePortDialog::on_ui_addPortButton_clicked(bool)
{
	bool hitOk;
	QString portName = QInputDialog::getText(this, "Add Port", "Port name: ", QLineEdit::Normal, QString(), &hitOk);
	
	if(!hitOk)
		return;
		
	m_portButtonList.push_back(new QRadioButton(portName));
	ui_portGroupBox->layout()->addWidget(m_portButtonList.back());
}

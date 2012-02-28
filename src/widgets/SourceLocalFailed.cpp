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

#include "SourceLocalFailed.h"

#include "MainWindow.h"
#include "SourceFile.h"

SourceLocalFailed::SourceLocalFailed(QWidget* parent) : QWidget(parent), m_sourceFile(0), m_state(Ask), m_filePath("")
{
	setupUi(this);
}

void SourceLocalFailed::setSourceFile(SourceFile* sourceFile)
{
	m_sourceFile = sourceFile;
}

void SourceLocalFailed::performAction(const QString filePath)
{
	m_filePath = filePath;
	
	if(m_state == Always && m_sourceFile->target()->hasDownload()) m_sourceFile->target()->download(m_filePath);
	else show();
}

void SourceLocalFailed::setState(int state)
{
	m_state = state;
}

int SourceLocalFailed::state()
{
	return m_state;
}

void SourceLocalFailed::on_ui_always_clicked()
{
	on_ui_yes_clicked();
	m_state = Always;
}

void SourceLocalFailed::on_ui_yes_clicked()
{
	hide();
	m_sourceFile->mainWindow()->setStatusMessage("Downloading Anyway...");
	QApplication::flush();
	m_sourceFile->mainWindow()->setStatusMessage(m_sourceFile->target()->download(m_filePath) ? tr("Download Succeeded") : tr("Download Failed"));
}

void SourceLocalFailed::on_ui_no_clicked() { hide(); }

void SourceLocalFailed::on_ui_never_clicked()
{
	on_ui_no_clicked();
	m_state = Never;
}
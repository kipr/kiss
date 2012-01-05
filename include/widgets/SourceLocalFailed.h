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

#ifndef _SOURCELOCALFAILED_H_
#define _SOURCELOCALFAILED_H_

#include <QWidget>
#include "ui_SourceLocalFailed.h"

class SourceFile;

class SourceLocalFailed : public QWidget, public Ui::SourceLocalFailed
{
Q_OBJECT
public:
	SourceLocalFailed(QWidget* parent);
	
	void performAction(const QString filePath);
	
	void setSourceFile(SourceFile* sourceFile);
	
	void setState(int state);
	int state();
	
	enum {
		Always,
		Ask,
		Never
	};
private slots:
	void on_ui_always_clicked();
	void on_ui_yes_clicked();
	void on_ui_no_clicked();
	void on_ui_never_clicked();

private:
	SourceFile* m_sourceFile;
	int m_state;
	
	QString m_filePath;
};

#endif

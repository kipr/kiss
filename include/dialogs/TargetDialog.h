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

#ifndef _TARGETDIALOG_H_
#define _TARGETDIALOG_H_

#include "ui_TargetDialog.h"

#include "TargetModel.h"
#include "InterfaceModel.h"

#include <QDialog>

class TargetDialog : public QDialog, private Ui::TargetDialog
{
Q_OBJECT
public:
	TargetDialog(InterfaceManager* manager, QWidget* parent = 0);
	~TargetDialog();
	
	TargetPtr target() const;
	
private slots:
	void currentTargetChanged(const QModelIndex& index);
	void targetChosen(const QModelIndex& index);
	void on_ui_interfaces_currentIndexChanged(int index);
	void on_ui_refresh_clicked();
	
private:
	InterfaceManager *m_manager;
	TargetModel m_model;
	InterfaceModel m_interfaceModel;
};

#endif

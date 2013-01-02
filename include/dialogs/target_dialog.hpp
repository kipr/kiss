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

#ifndef _TARGET_DIALOG_HPP_
#define _TARGET_DIALOG_HPP_

#include "ui_TargetDialog.h"

#include "target_model.hpp"
#include "interface_model.hpp"
#include "interface_manager.hpp"

#include <QDialog>

namespace Kiss
{
	namespace Dialog
	{
		class Target : public QDialog, private Ui::TargetDialog
		{
		Q_OBJECT
		public:
			Target(Kiss::Target::InterfaceManager *manager, QWidget *parent = 0);
			~Target();

			Kiss::Target::TargetPtr target() const;

		private slots:
			void currentTargetChanged(const QModelIndex& index);
			void targetChosen(const QModelIndex& index);
			void on_ui_interfaces_currentIndexChanged(int index);
			void on_ui_refresh_clicked();
			void on_ui_manual_clicked();

		private:
			Kiss::Target::InterfaceManager *m_manager;
			Kiss::Target::TargetModel m_model;
			Kiss::Target::InterfaceModel m_interfaceModel;
			Kiss::Target::TargetPtr m_manualTarget;
		};
	}
}

#endif

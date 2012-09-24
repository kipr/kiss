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

#ifndef _SOURCEFINDWIDGET_H_
#define _SOURCEFINDWIDGET_H_

#include <QWidget>
#include "ui_SourceFindWidget.h"

namespace Kiss
{
	namespace Widget
	{
		class SourceFile;

		class SourceFind : public QWidget, public Ui::SourceFindWidget
		{
		Q_OBJECT
		public:
			SourceFind(QWidget *parent);

			void setSourceFile(SourceFile *sourceFile);

			void setModified(const bool& m);
			bool isModified() const;

		public slots:
			void show();

		private slots:
			void on_ui_next_clicked();
			void on_ui_find_textChanged(const QString& text);
			void on_ui_matchCase_stateChanged(int state);
			void on_ui_replaceNext_clicked();
			void on_ui_replaceAll_clicked();

		private:
			SourceFile* m_sourceFile;

			bool m_findModified;
		};
	}
}

#endif

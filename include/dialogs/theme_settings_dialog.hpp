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

#ifndef _THEME_SETTINGS_DIALOG_HPP_
#define _THEME_SETTINGS_DIALOG_HPP_

#include "color_box.hpp"

#include <QDialog>
#include <QMap>
#include <QAbstractButton>

#define LEXER "Lexer"
#define FONT "font"
#define FONT_SIZE "fontsize"
#define BACKGROUND_COLOR "backgroundcolor"

namespace Ui
{
	class ThemeSettingsDialog;
}

namespace Kiss
{
	namespace Dialog
	{
		class ThemeSettings : public QDialog
		{
		Q_OBJECT
		public:
			ThemeSettings(QWidget *parent = 0);
			~ThemeSettings();

			virtual int exec();

			const QMap<QString, QColor>& settings() const;
			QColor backgroundColor() const;
			QFont font() const;
			int fontSize() const;

			void readSettings();
			void writeSettings();

			static void initializeDefaults();

		public slots:
			void setDefaults();

		private slots:
			void settingChanged(QColor color);

		private:
			void updateBoxes();

			::Ui::ThemeSettingsDialog *ui;
			QMap<QString, QColor> m_lexerSettings;
			ColorBox **m_boxes;
			int m_fontSize;
			QFont m_font;
			QColor m_backgroundColor;
		};
	}
}

#endif

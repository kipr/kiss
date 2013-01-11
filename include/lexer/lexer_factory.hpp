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

#ifndef _LEXER_FACTORY_HPP_
#define _LEXER_FACTORY_HPP_

#include "singleton.hpp"

#include <QMap>
#include <QFont>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

#include <QDebug>

namespace Kiss
{
	namespace Lexer
	{
		class Base;

		class Constructor : public QObject
		{
		Q_OBJECT
		public:
			Constructor(const QString& name);
			virtual ~Constructor();

			const QString& name() const;

			virtual Base *construct() const = 0;
			virtual Base *construct(const QString& apis) const = 0;

			virtual Base *_new() const = 0;
			virtual void _delete(Base *base) const = 0;

			virtual QStringList extensions() const = 0;
			
		private:
			QString m_name;
		};

		class Base
		{
		public:
			Base(QsciLexer *lexer, const Constructor *constructor)
				: m_lexer(lexer),
				m_constructor(constructor),
				m_apis(lexer)
			{	
			}
			
			virtual ~Base();
			
			const Constructor *constructor() const
			{
				return m_constructor;
			}

			void setAPIFile(const QString& apis)
			{
				if(m_apis.load(apis)) m_apis.prepare();
			}
			
			QsciLexer *lexer() const
			{
				return m_lexer;
			}

			virtual const bool cStyleBlocks() const = 0;
		private:
			QsciLexer* m_lexer;
			const Constructor* m_constructor;
			QsciAPIs m_apis;
		};

		class Settings : public Singleton<Settings>
		{
		public:
			Settings();
			~Settings();

			QColor getSetting(const QString& type, const QColor& defaultValue) { return settings.value(type, defaultValue); }
			QMap<QString, QColor> getSettings() { return settings; }

			void setSetting(const QString& type, const QColor& color) { settings.insert(type, color); }
			void setSettings(QMap<QString, QColor> newSettings) { settings = newSettings; }

		private:
			QMap<QString, QColor> settings;
		};

		class Factory : public Singleton<Factory>
		{
		public:
			Factory();
			~Factory();

			Base *newLexer(const QString& ext) const;
			Base *newLexerFromConstructor(const Constructor* constructor) const;
			Constructor *constructor(const QString& ext) const;

			void registerLexerConstructor(Constructor *c);
			void registerLexerConstructor(Constructor *c, const QStringList& exts);
			void unregisterLexerConstructor(Constructor *c);

			static void setAPIsForLexer(Base *lexer, const QString& apis);
			static bool isLexerFromConstructor(Base *lexer, Constructor* constructor);

			void setFont(const QFont& font);
			QFont font() const;

			const QStringList extensions() const;
			const QStringList formattedExtensions() const;
		private:
			QFont m_font;
			QMap<QString, Constructor *> m_constructors;
		};
	}
}

Q_DECLARE_INTERFACE(Kiss::Lexer::Constructor, "com.kipr.kiss.LexerConstructor/1.0");

#endif

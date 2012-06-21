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

#ifndef _TEMPLATEMANAGER_H_
#define _TEMPLATEMANAGER_H_

#include <QString>
#include <QIcon>

#include "Kiss.h"
#include "Singleton.h"

class TemplateManager : public Singleton<TemplateManager>
{
public:
	QStringList types() const;
	
	QStringList templateFolders(const QString& type, const QString& _template);
	QStringList templates(const QString& type, const QString& _template);
	QIcon templateIcon(const QString& type, const QString& _template);
	
	QStringList userTemplates(const QString& type);
	bool addUserTemplate(const QString& type, const QString& name, const QString& content);
	bool deleteUserTemplate(const QString& type, const QString& name);
	
	QString pathForUserTemplate(const QString& type, const QString& _template);
	QString pathForTemplate(const QString& type, const QString& _template);
	
	bool isTemplateDirectory(const QString& type, const QString& _template);
	bool isTemplateDirectory(const QString& path);
	
	static const QString& defaultTemplateName();
private:
	void ensureUserTemplateDirExists(const QString& type);
	
	static QString m_defaultName;
};

#endif

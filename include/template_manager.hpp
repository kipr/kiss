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

#ifndef _TEMPLATE_MANAGER_HPP_
#define _TEMPLATE_MANAGER_HPP_

#include <QString>
#include <QIcon>
#include <QMap>

#include "kiss.hpp"
#include "singleton.hpp"

#define KISS_TEMPLATE_PACK_EXT "ktp"
#define KISS_TEMPLATE_PACK_FILTER ("KISS Template Pack (*." KISS_TEMPLATE_PACK_EXT ")")

class TemplateManager : public Kiss::Singleton<TemplateManager>
{
public:
	TemplateManager();
	~TemplateManager();
	
	QStringList types() const;
	
	QStringList templateFolders(const QString& type, const QString& _template);
	QStringList templates(const QString& type, const QString& _template);
	QIcon templateIcon(const QString& type, const QString& _template);
	
	bool addUserTemplate(const QString& type, const QString& name, const QString& content);
	bool deleteUserTemplate(const QString& type, const QString& name);
	
	QStringList userTemplateTypes() const;
	QStringList userTemplates(const QString& type);
	QString pathForUserTemplate(const QString& type, const QString& _template);
	
	QString pathForTemplate(const QString& type, const QString& _template);
	
	bool isTemplateDirectory(const QString& type, const QString& _template);
	bool isTemplateDirectory(const QString& path);
	
	static const QString& defaultTemplateName();
	
private:
	void ensureUserTemplateDirExists(const QString& type);
	QString packPath() const;
	
	static QString m_defaultName;
};

#endif

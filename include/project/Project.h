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

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <QString>
#include <QIODevice>
#include <QObject>
#include <QMap>

#include "WorkingUnit.h"

class QTinyArchive;
class TinyArchiveReader;
class TinyArchiveWriter;

#include "ArchiveWriter.h"

#define TARGET_KEY "TARGET"
#define SETTINGS_ID 1

typedef QMap<QString, QString> QStringMap;

class Project : public QObject, public WorkingUnit
{
Q_OBJECT
public:
	~Project();
	
	const TinyNode* addFile(const QString& path);
	void setName(const QString& name);
	
	void refresh();
	
	QStringList files() const;
	
	QStringList resources() const;
	QStringList sources() const;
	
	QString outputPath() const;
	
	const QString& name() const;
	
	static Project* load(const QString& path);
	static Project* create(const QString& path);
	
	QTinyArchive* archive() const;
	
	const bool updateSetting(const QString& key, const QString& value);
	const bool removeSetting(const QString& key);
	void setSettings(const QStringMap& settings);
	QStringMap settings() const;
	void setTargetName(const QString& target);
	
	const bool sync();
	
signals:
	void settingUpdated(const QString& key);
	void settingRemoved(const QString& key);
	void settingsChanged();
	
protected:
	Project(TinyArchiveReader* reader, TinyArchiveWriter* writer);
	Project(TinyArchiveWriter* writer);
	
private:
	void processSettings(const QStringMap& settings);
	
	QString m_path;
	QString m_name;
	
	QTinyArchive* m_archive;
	TinyArchiveWriter* m_writer;
	
	QString m_associatedPort;
};

Q_DECLARE_METATYPE(Project*)

#endif

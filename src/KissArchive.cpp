/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#include "KissArchive.h"
#include "Os.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QString>

#define INSTALLED_VERSION_STRING "/version"
#define INSTALLED_DIRS_STRING "/dirs"
#define INSTALLED_FILES_STRING "/files"

const static char kissMagic[2] = {0xB3, 0x7A};
const static unsigned kissVersion = KISS_ARCHIVE_VERSION;

/**
 * Writes a KISS Archive to the QIODevice specified
 *
 * Contents of a created archive: (Integer - 4 Byte Unsigned; String - x Bytes)
 * 
 * 2 Bytes - 0xB37A magic
 * Integer - Number of platforms
 * numPlatforms * 3 Bytes - Platforms supported (3 chars per platform)
 * Integer - KISS Archiver version
 * Integer - Length of Name String
 * String - Name
 * Integer - Package version
 * Integer - Number of Files
 * for(0 to numberFiles) [
 *    Integer - File Name length
 *    String - File Name
 *    Integer - File length
 *    String - File data
 * ]
 *
 */
KissReturn KissArchive::create(const QString& name, unsigned pVersion, const QStringList& platforms, const QStringList& files, QIODevice* out)
{
	QStringList noBlanks;
	foreach(const QString& str, files) {
		if(!str.isEmpty()) noBlanks << str;
	}
	
	unsigned size = (unsigned)noBlanks.size();
	out->write(kissMagic, 2);
	
	unsigned numPlatforms = platforms.size();
	out->write((char*)&numPlatforms, sizeof(unsigned));
	foreach(const QString& platform, platforms) {
		if(platform.size() != 3) qWarning() << "Platform" << platform << "not 3 bytes";
		out->write(platform.toLocal8Bit(), 3);
	}
	
	out->write((char*)&kissVersion, sizeof(unsigned));
	unsigned nameSize = (unsigned)name.length();
	out->write((char*)&nameSize, sizeof(unsigned));
	out->write(name.toLocal8Bit(), nameSize);
	out->write((char*)&pVersion, sizeof(unsigned));
	
	out->write((char*)&size, sizeof(unsigned));
	foreach(const QString& file, noBlanks) {
		if(file.isEmpty()) continue;
		QFile f(file);
		if (!f.open(QIODevice::ReadOnly)) return KissReturn(true, QObject::tr("Unable to open ") + file + QObject::tr(" for reading."));
		
		unsigned strLength = file.length();
		out->write((char*)&strLength, sizeof(unsigned));
		out->write(file.toLocal8Bit());
		
		const QByteArray& data = qCompress(f.readAll());
		unsigned dataLength = data.size();
		out->write((char*)&dataLength, sizeof(unsigned));
		out->write(data);
	}
	return KissReturn(false);
}

/**
 * Install a package given from a QIODevice
 */
KissReturn KissArchive::install(QIODevice* in)
{
	QStringList files;
	QStringList dirs;
	
	// Reads the file's "magic" to make sure we have a Kiss Archive
	char magic[2];
	in->read(magic, 2);
	if(magic[0] != kissMagic[0] || magic[1] != kissMagic[1]) {
		qWarning() << "Bad Magic";
		return KissReturn(true, QObject::tr("Bad Magic. Probably not a KISS Archive"));
	}
	
	// Read platforms, halt if current platform not detected
	unsigned numPlatforms = 0;
	in->read((char*)&numPlatforms, sizeof(unsigned));
	bool match = false;
	for(unsigned i = 0; i < numPlatforms; ++i) {
		if(QString(in->read(3).data()) == OS_NAME) {
			match = true;
		}
	}
	if(!match) {
		qWarning() << "Incorrect OS";
		return KissReturn(true, QObject::tr("This OS is not supported by the archive"));
	}
	
	// Checks the Kiss Archive Specification version, so we know how to extract
	unsigned version = 0;
	in->read((char*)&version, sizeof(unsigned));
	if(kissVersion != version) {
		qWarning() << "Version mismatch. Expected:" << kissVersion << ", got" << version;
		return false;
	}
	
	// Reads archive name and internal version
	unsigned nameSize = 0;
	in->read((char*)&nameSize, sizeof(unsigned));
	QString name(in->read(nameSize).data());
	unsigned pVersion = 0;
	in->read((char*)&pVersion, sizeof(unsigned));
	
	if(KissArchive::version(name) >= pVersion) {
		qWarning() << "Higher version already installed. Skipping.";
		return KissReturn(true, QObject::tr("Higher version of same archive already installed"));
	} else if(KissArchive::version(name) < pVersion) {
		uninstall(name);
	}
	
	// Recursively extract files and dirs
	unsigned numFiles = 0;
	in->read((char*)&numFiles, sizeof(unsigned));
	for(unsigned i = 0; i < numFiles; ++i) {
		unsigned strLength = 0;
		in->read((char*)&strLength, sizeof(unsigned));
		QString str = QString(in->read(strLength).data());
		
		files << str;
		
		unsigned dataLength = 0;
		in->read((char*)&dataLength, sizeof(unsigned));
		const QByteArray& data = qUncompress(in->read(dataLength));
		
		if(str.isEmpty()) continue;
		
		QFile f(str);
		const QString& filePath = QFileInfo(str).path();
		QDir dir;
		if(!dir.exists(filePath)) {
			dir.mkpath(filePath);
			dirs.prepend(filePath);
		}
		if(!f.open(QIODevice::WriteOnly)) {
			qWarning() << "Unable to open" << str << "for writing.";
		}
		f.write(data);
	}
	qWarning() << files;
	
	QSettings installed(KISS_ARCHIVE_FILE, QSettings::IniFormat);
	installed.setValue(name + INSTALLED_VERSION_STRING, pVersion);
	installed.setValue(name + INSTALLED_FILES_STRING, files);
	installed.setValue(name + INSTALLED_DIRS_STRING, dirs);
	installed.sync();
	
	return KissReturn(false);
}

/**
 * Uninstalls a package from KISS's directory structure
 */
KissReturn KissArchive::uninstall(const QString& name)
{
	QSettings installed(KISS_ARCHIVE_FILE, QSettings::IniFormat);
	if(!installed.childGroups().contains(name)) return KissReturn(true, QObject::tr("No such archive installed"));
	
	const QStringList& files = installed.value(name + INSTALLED_FILES_STRING).toStringList();
	foreach(const QString& file, files) {
		if(!QFile(file).remove()) qWarning() << "Failed to remove file" << file;
		const QDir& dir = QFileInfo(file).dir();
		if(dir.entryList(QDir::NoDotAndDotDot).size() == 0) dir.rmdir(dir.absolutePath());
	}
	
	const QStringList& dirs = installed.value(name + INSTALLED_DIRS_STRING).toStringList();
	foreach(const QString& dir, dirs) {
		QDir().rmdir(dir);
	}
	installed.remove(name);
	installed.sync();
	
	return KissReturn(false);
}

QStringList KissArchive::list(QIODevice* in)
{
	QStringList files;
	QStringList dirs;
	
	// Reads the file's "magic" to make sure we have a Kiss Archive
	char magic[2];
	in->read(magic, 2);
	if(magic[0] != kissMagic[0] || magic[1] != kissMagic[1]) {
		qWarning() << "Bad Magic";
		return QStringList();
	}
	
	// Read platforms, halt if current platform not detected
	unsigned numPlatforms = 0;
	in->read((char*)&numPlatforms, sizeof(unsigned));
	in->read(numPlatforms * 3);
	
	// Checks the Kiss Archive Specification version, so we know how to extract
	unsigned version = 0;
	in->read((char*)&version, sizeof(unsigned));
	if(kissVersion != version) {
		qWarning() << "Version mismatch. Expected:" << kissVersion << ", got" << version;
		return QStringList();
	}
	
	// Reads archive name and internal version
	unsigned nameSize = 0;
	in->read((char*)&nameSize, sizeof(unsigned));
	QString name(in->read(nameSize).data());
	unsigned pVersion = 0;
	in->read((char*)&pVersion, sizeof(unsigned));
	
	// Recursively extract files and dirs
	unsigned numFiles = 0;
	in->read((char*)&numFiles, sizeof(unsigned));
	for(unsigned i = 0; i < numFiles; ++i) {
		unsigned strLength = 0;
		in->read((char*)&strLength, sizeof(unsigned));
		QString str = QString(in->read(strLength).data());
		
		files << str;
		
		unsigned dataLength = 0;
		in->read((char*)&dataLength, sizeof(unsigned));
		in->read(dataLength);
	}
	
	return files;
}

/**
 * Returns the package's version, if that package is installed
 */
const unsigned KissArchive::version(const QString& name)
{
	return QSettings(KISS_ARCHIVE_FILE, QSettings::IniFormat).value(name + INSTALLED_VERSION_STRING, 0).toUInt();
}

/**
 * List the installed packages, as declared in the file "installed"
 */
QStringList KissArchive::installed()
{
	return QSettings(KISS_ARCHIVE_FILE, QSettings::IniFormat).childGroups();
}

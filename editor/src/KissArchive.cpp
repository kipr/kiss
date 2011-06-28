#include "KissArchive.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSettings>

const static char kissMagic[2] = {0xB3, 0x7A};
const static unsigned kissVersion = KISS_ARCHIVE_VERSION;

template <typename T>
QList<T> reversed( const QList<T> & in ) {
    QList<T> result;
    std::reverse_copy( in.begin(), in.end(), std::back_inserter( result ) );
    return result;
}

bool KissArchive::create(const QString& name, unsigned pVersion, const QStringList& files, QIODevice* out)
{
	QStringList noBlanks;
	foreach(const QString& str, files) {
		if(!str.isEmpty()) noBlanks << str;
	}
	
	unsigned size = (unsigned)noBlanks.size();
	out->write(kissMagic, 2);
	out->write((char*)&kissVersion, sizeof(unsigned));
	unsigned nameSize = (unsigned)name.length();
	out->write((char*)&nameSize, sizeof(unsigned));
	out->write(name.toLocal8Bit(), nameSize);
	out->write((char*)&pVersion, sizeof(unsigned));
	
	out->write((char*)&size, sizeof(unsigned));
	foreach(const QString& file, noBlanks) {
		if(file.isEmpty()) continue;
		QFile f(file);
		if (!f.open(QIODevice::ReadOnly)) return false;
		
		unsigned strLength = file.length();
		out->write((char*)&strLength, sizeof(unsigned));
		out->write(file.toLocal8Bit());
		
		const QByteArray& data = qCompress(f.readAll());
		unsigned dataLength = data.size();
		out->write((char*)&dataLength, sizeof(unsigned));
		out->write(data);
	}
	return true;
}

bool KissArchive::install(QIODevice* in)
{
	QStringList files;
	QStringList dirs;
	
	// Reads the file's "magic" to make sure we have a Kiss Archive
	char magic[2];
	in->read(magic, 2);
	if(magic[0] != kissMagic[0] || magic[1] != kissMagic[1]) {
		qWarning() << "Bad Magic";
		return false;
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
		return true;
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
		
		
		QFile f(str);
		const QString& filePath = QFileInfo(str).path();
		QDir dir(filePath);
		if(!dir.exists()) {
			dir.mkpath(filePath);
			dirs.prepend(filePath);
		}
		if(!f.open(QIODevice::WriteOnly)) {
			qWarning() << "Unable to open" << str << "for writing.";
			return false;
		}
		f.write(data);
	}
	qWarning() << files;
	
	QSettings installed(KISS_ARCHIVE_FILE, QSettings::IniFormat);
	installed.setValue(name + "/version", pVersion);
	installed.setValue(name + "/files", files);
	installed.setValue(name + "/dirs", dirs);
	installed.sync();
	
	return true;
}

bool KissArchive::uninstall(const QString& name)
{
	QSettings installed(KISS_ARCHIVE_FILE, QSettings::IniFormat);
	if(!installed.childGroups().contains(name)) return false;
	
	const QStringList& files = installed.value(name + "/files").toStringList();
	foreach(const QString& file, files) {
		if(!QFile(file).remove()) qWarning() << "Failed to remove file" << file;
		const QDir& dir = QFileInfo(file).dir();
		if(dir.entryList(QDir::NoDotAndDotDot).size() == 0) dir.rmdir(dir.absolutePath());
	}
	
	const QStringList& dirs = installed.value(name + "/dirs").toStringList();
	foreach(const QString& dir, dirs) {
		if(!QDir().rmdir(dir)) qWarning() << "Unable to rm" << dir;
	}
	installed.remove(name);
	installed.sync();
	
	return true;
}

const unsigned KissArchive::version(const QString& name)
{
	return QSettings(KISS_ARCHIVE_FILE, QSettings::IniFormat).value(name + "/version", 0).toUInt();
}

QStringList KissArchive::installed()
{
	return QSettings(KISS_ARCHIVE_FILE, QSettings::IniFormat).childGroups();
}

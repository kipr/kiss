#include "KissArchive.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

const static char kissMagic[2] = {0xB3, 0x7A};

bool KissArchive::create(const QStringList& files, QIODevice* out)
{
	QStringList noBlanks;
	foreach(const QString& str, files) {
		if(!str.isEmpty()) noBlanks << str;
	}
	
	int size = noBlanks.size();
	out->write(kissMagic, 2);
	
	out->write((char*)&size, sizeof(int));
	foreach(const QString& file, noBlanks) {
		if(file.isEmpty()) continue;
		QFile f(file);
		if (!f.open(QIODevice::ReadOnly)) return false;
		
		int strLength = file.length();
		out->write((char*)&strLength, sizeof(int));
		out->write(file.toLocal8Bit());
		
		const QByteArray& data = f.readAll();
		int dataLength = data.size();
		out->write((char*)&dataLength, sizeof(int));
		out->write(data);
	}
	return true;
}

bool KissArchive::extract(QIODevice* in)
{
	QStringList files;
	int numFiles = 0;
	char magic[2];
	in->read(magic, 2);
	if(magic[0] != kissMagic[0] || magic[1] != kissMagic[1]) {
		qWarning() << "Bad Magic";
		return false;
	}
	
	in->read((char*)&numFiles, sizeof(int));
	for(int i = 0; i < numFiles; ++i) {
		int strLength = 0;
		in->read((char*)&strLength, sizeof(int));
		QString str = QString(in->read(strLength).data());
		files << str;
		int dataLength = 0;
		in->read((char*)&dataLength, sizeof(int));
		const QByteArray& data = in->read(dataLength);
		
		QFile f(str);
		QDir("").mkpath(QFileInfo(str).absolutePath());
		if(!f.open(QIODevice::WriteOnly)) {
			qWarning() << "Unable to open" << str << "for writing.";
			return false;
		}
		f.write(data);
	}
	qWarning() << files;
	return true;
}
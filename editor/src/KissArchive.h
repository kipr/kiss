#ifndef __KISSARCHIVE_H__
#define __KISSARCHIVE_H__

#include <QStringList>
#include <QIODevice>

#define KISS_ARCHIVE_VERSION 1
#define KISS_ARCHIVE_FILE "installed"

class KissArchive 
{
public:
	static bool create(const QString& name, unsigned version, const QStringList& platforms, const QStringList& files, QIODevice* out);
	static bool install(QIODevice* in);
	static bool uninstall(const QString& name);
	static const unsigned version(const QString& name);
	static QStringList installed();
	static QString osName();
private:
};

#endif
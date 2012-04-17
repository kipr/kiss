#ifndef _KISSRUNNABLEARCHIVE_H_
#define _KISSRUNNABLEARCHIVE_H_

#include "TinyArchive.h"
#include <QString>

class KissRunnableArchiveWriter
{
public:
	KissRunnableArchiveWriter();
	
	void setCommand(const QString& command);
	
	bool addFile(const QString& path);
	void addVirtualFile(const QString& file, const QByteArray& contents);
	
	const TinyArchive& tinyArchive() const;
	
private:
	TinyArchive m_tinyArchive;
};

class KissRunnableArchiveReader
{
public:
	KissRunnableArchiveReader(TinyArchive* tinyArchive);
	
	bool extractTo(const QString& path);
	QString command() const;
	
	// TODO: Add code to start QProcess
private:
	TinyArchive* m_tinyArchive;
};

#endif

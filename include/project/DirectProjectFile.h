#ifndef _DIRECTPROJECTFILE_H_
#define _DIRECTPROJECTFILE_H_

#include "ProjectFile.h"

#include "QueuedIO.h"

#include <QDir>

class DirectProjectFile : public ProjectFile
{
public:
	DirectProjectFile(const QString& path);
	
	virtual QStringList list() const;
	virtual const QString& path() const;

	virtual void addFile(const QString& path, QDataStream& file);
	virtual void addFile(const QString& path, const QByteArray& data);

	virtual QByteArray fileContents(const QString& path) const;
	virtual bool updateFileContents(const QString& path, const QByteArray& ba);

	virtual void removeFile(const QString& path);
	virtual void removeIndex(const quint32& i);

	virtual void setProjectSettings(const QMap<QString, QString>& projectSettings);
	virtual QMap<QString, QString> projectSettings() const;

	virtual bool sync();
	
	QString projectSettingsFile() const;

private:
	QStringList list(QDir dir) const;
	
	QueuedIO m_io;
	QString m_path;
	
	QMap<QString, QString> m_projectSettings;
};

#endif

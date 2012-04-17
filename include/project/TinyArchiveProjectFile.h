#ifndef _TINYARCHIVEPROJECTFILE_H_
#define _TINYARCHIVEPROJECTFILE_H_

#include "TinyArchive.h"
#include "ProjectFile.h"

#include <QStringList>

class TinyArchiveProjectFile : public ProjectFile
{
public:
	TinyArchiveProjectFile();
	
	virtual bool init(const QString& path);
	virtual bool load(const QString& path);
	
	virtual QStringList list() const;
	virtual const QString& path() const;
	
	virtual void addFile(const QString& path, QDataStream& file);
	virtual void addFile(const QString& path, const QByteArray& data);
	
	virtual QByteArray fileContents(const QString& path) const;
	virtual bool updateFileContents(const QString& path, const QByteArray& ba);
	
	virtual void removeFile(const QString& path);
	virtual void removeIndex(const quint32& i);
	
	bool containsSetting(const QString& key) const;
	QString valueForSetting(const QString& key) const;
	void addProjectSetting(const QString& key, const QString& value);
	void removeProjectSetting(const QString& key, const QString& value);
	
	virtual void setProjectSettings(const QMap<QString, QString>& projectSettings);
	virtual QMap<QString, QString> projectSettings() const;

	virtual bool sync();
private:
	QByteArray contentsForEntry(const ArchiveEntry* entry) const;
	
	TinyArchive* m_archive;
	
	QString m_path;
};

#endif

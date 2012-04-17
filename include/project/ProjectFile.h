#ifndef _PROJECTFILE_H_
#define _PROJECTFILE_H_

#include <QString>
#include <QList>
#include <QDataStream>
#include <QByteArray>
#include <QMap>
#include <QObject>

struct ProjectFile : public QObject
{
Q_OBJECT
public:
	virtual ~ProjectFile();
	
	virtual bool init(const QString& path) = 0;
	virtual bool load(const QString& path) = 0;
	
	virtual QStringList list() const = 0;
	virtual const QString& path() const = 0;
	
	virtual void addFile(const QString& path, QDataStream& file) = 0;
	virtual void addFile(const QString& path, const QByteArray& data) = 0;
	
	virtual QByteArray fileContents(const QString& path) const = 0;
	virtual bool updateFileContents(const QString& path, const QByteArray& ba) = 0;
	
	void moveFile(const QString& oldPath, const QString& newPath);
	
	bool containsFile(const QString& path) const;
	
	virtual void removeFile(const QString& path) = 0;
	virtual void removeIndex(const quint32& i) = 0;
	
	bool containsSetting(const QString& key) const;
	QString valueForSetting(const QString& key) const;
	void addProjectSetting(const QString& key, const QString& value);
	void removeProjectSetting(const QString& key);
	
	virtual void setProjectSettings(const QMap<QString, QString>& projectSettings) = 0;
	virtual QMap<QString, QString> projectSettings() const = 0;

	virtual bool sync() = 0;
	
public slots:
	void settingRemoved(const QString& key);
	void settingUpdated(const QString& key, const QString& value);
	
signals:
	void fileCreated(const QString& file);
	void fileChanged(const QString& file);
	void fileRemoved(const QString& file);
};

#endif

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <QString>
#include <QIODevice>
#include <QObject>

class Project : public QObject
{
Q_OBJECT
public:	
	bool addFile(const QString& path);
	bool createFile(const QString& path);
	const QString& setName(const QString& name);
	
	void refresh();
	
	QStringList files() const;
	
	QStringList resources() const;
	QStringList sources() const;
	
	QString outputPath() const;
	
	const QString& name();
	
	static Project* load(const QString& path);
	static Project* create(const QString& path);
	
signals:
	void updated();
	
protected:
	Project(const QString& name);
	
private:
	QString m_path;
};

#endif

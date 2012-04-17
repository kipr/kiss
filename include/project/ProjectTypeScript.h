#ifndef _PROJECTTYPESCRIPT_H_
#define _PROJECTTYPESCRIPT_H_

#include "ProjectType.h"

#include <QIcon>

class ProjectTypeScript : public QObject, public ProjectType
{
Q_OBJECT
Q_PROPERTY(QString defaultExtension READ defaultExtension WRITE setDefaultExtension)
Q_PROPERTY(QStringList defaultSettings READ defaultSettings WRITE setDefaultSettings)
Q_PROPERTY(QString name READ name WRITE setName)
public:
	ProjectTypeScript();
	
	void setName(const QString& name);
	
	void setDefaultExtension(const QString& defaultExtension);
	const QString& defaultExtension() const;
	
	void setDefaultSettings(const QStringList& defaultSettings);
	const QStringList& defaultSettings() const;
	
	void setIcon(const QString& file);
	
	Q_INVOKABLE virtual const QIcon& icon() const;
private:
	QString m_defaultExtension;
	QStringList m_defaultSettings;
	
	QIcon m_icon;
};


#endif

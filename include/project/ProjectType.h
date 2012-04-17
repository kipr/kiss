#ifndef _PROJECTTYPE_H_
#define _PROJECTTYPE_H_

#include "Singleton.h"

class ProjectType;

#include <QObject>
#include <QMetaType>
#include <QList>
#include <QStringList>

class ProjectTypes : public QObject, public Singleton<ProjectTypes>
{
Q_OBJECT
public:
	ProjectTypes();
	~ProjectTypes();
	
	Q_INVOKABLE void addProjectType(ProjectType* projectType);
	Q_INVOKABLE QList<ProjectType*> projectTypes() const;
private:
	QList<ProjectType*> m_projectTypes;
};

class ProjectType
{
public:
	ProjectType(const QString& name);
	virtual ~ProjectType();
	
	const QString& name() const;
	
	virtual const QStringList& defaultSettings() const = 0;
	virtual const QString& defaultExtension() const = 0;
	virtual const QIcon& icon() const = 0;
protected:
	QString m_name;
};

Q_DECLARE_METATYPE(ProjectType*);

#endif

#include "ProjectType.h"

#include <QIcon>

struct BlankProjectType : ProjectType
{
	BlankProjectType() : ProjectType("Blank") {}
	
	const QStringList& defaultSettings() const { return m_defaultSettings; }
	const QString& defaultExtension() const { return m_defaultExtension; }
	const QIcon& icon() const { return m_icon; }
	
private:
	QStringList m_defaultSettings;
	QString m_defaultExtension;
	
	QIcon m_icon;
};

ProjectTypes::ProjectTypes()
{
	addProjectType(new BlankProjectType);
}

ProjectTypes::~ProjectTypes()
{
	foreach(ProjectType* projectType, m_projectTypes) delete projectType;
}

void ProjectTypes::addProjectType(ProjectType* projectType)
{
	m_projectTypes.append(projectType);
}

QList<ProjectType*> ProjectTypes::projectTypes() const
{
	return m_projectTypes;
}

ProjectType::ProjectType(const QString& name) : m_name(name)
{
}

ProjectType::~ProjectType()
{
	
}

const QString& ProjectType::name() const
{
	return m_name;
}
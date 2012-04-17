#include "ProjectTypeScript.h"

#include "ResourceHelper.h"

ProjectTypeScript::ProjectTypeScript() : ProjectType("Set name in js!")
{
	
}

void ProjectTypeScript::setName(const QString& name)
{
	m_name = name;
}

void ProjectTypeScript::setDefaultExtension(const QString& defaultExtension)
{
	m_defaultExtension = defaultExtension;
}

const QString& ProjectTypeScript::defaultExtension() const
{
	return m_defaultExtension;
}

void ProjectTypeScript::setDefaultSettings(const QStringList& defaultSettings)
{
	m_defaultSettings = defaultSettings;
}

const QStringList& ProjectTypeScript::defaultSettings() const
{
	return m_defaultSettings;
}

void ProjectTypeScript::setIcon(const QString& file)
{
	m_icon = ResourceHelper::ref().icon(file);
}

const QIcon& ProjectTypeScript::icon() const
{
	return m_icon;
}
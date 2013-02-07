#include "language_helper_manager.hpp"

using namespace Kiss;

LanguageHelperManager::LanguageHelperManager()
{
}

LanguageHelperManager::~LanguageHelperManager()
{
	// TODO: Delete? Plugins will delete themselves.
}

void LanguageHelperManager::addLanguageHelper(LanguageHelper *languageHelper)
{
	if(m_languageHelpers.contains(languageHelper)) return;
	m_languageHelpers.append(languageHelper);
}

void LanguageHelperManager::removeLanguageHelper(LanguageHelper *languageHelper)
{
	m_languageHelpers.removeAll(languageHelper);
}

QList<LanguageHelper *> LanguageHelperManager::languageHelpers() const
{
	return m_languageHelpers;
}

Compiler::OutputList LanguageHelperManager::preprocess(const Kiss::KarPtr &archive, const QStringList &searchPaths) const
{
	Compiler::OutputList ret;
	foreach(const LanguageHelper *const languageHelper, m_languageHelpers) {
		ret += languageHelper->preprocess(archive, searchPaths);
	}
	return ret;
}
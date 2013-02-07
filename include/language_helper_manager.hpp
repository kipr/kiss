#ifndef _LANGUAGE_HELPER_MANAGER_HPP_
#define _LANGUAGE_HELPER_MANAGER_HPP_

#include "singleton.hpp"
#include "language_helper.hpp"

#include <QList>

namespace Kiss
{
	class LanguageHelperManager : public Singleton<LanguageHelperManager>
	{
	public:
		LanguageHelperManager();
		~LanguageHelperManager();
		
		void addLanguageHelper(LanguageHelper *languageHelper);
		void removeLanguageHelper(LanguageHelper *languageHelper);
		QList<LanguageHelper *> languageHelpers() const;
		
		Compiler::OutputList preprocess(const Kiss::KarPtr &archive, const QStringList &searchPaths) const;
		
	private:
		QList<LanguageHelper *> m_languageHelpers;
	};
}

#endif

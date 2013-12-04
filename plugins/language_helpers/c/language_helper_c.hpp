#ifndef _LANGUAGE_HELPER_C_HPP_
#define _LANGUAGE_HELPER_C_HPP_

#include "language_helper.hpp"

namespace kiss
{
	class LanguageHelperC : public LanguageHelper
	{
	Q_OBJECT
  Q_PLUGIN_METADATA(IID "com.kipr.kiss.LanguageHelper/1.0" FILE "language_helper_c.json")
	Q_INTERFACES(kiss::LanguageHelper);
	public:
		virtual Compiler::OutputList preprocess(const kiss::KarPtr &archive, const QStringList &searchPaths) const;
	};
}

#endif

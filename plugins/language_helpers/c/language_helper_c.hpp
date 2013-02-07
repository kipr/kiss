#ifndef _LANGUAGE_HELPER_C_HPP_
#define _LANGUAGE_HELPER_C_HPP_

#include "language_helper.hpp"

namespace Kiss
{
	class LanguageHelperC : public LanguageHelper
	{
	Q_OBJECT
	Q_INTERFACES(Kiss::LanguageHelper);
	public:
		virtual Compiler::OutputList preprocess(const Kiss::KarPtr &archive, const QStringList &searchPaths) const;
	};
}

#endif

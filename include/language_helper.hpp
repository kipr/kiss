#ifndef _LANGUAGE_HELPER_HPP_
#define _LANGUAGE_HELPER_HPP_

#include <QObject>
#include <QStringList>

#include <kar.hpp>
#include <pcompiler/output.hpp>

namespace Kiss
{
	class LanguageHelper : public QObject
	{
	Q_OBJECT
	public:
		virtual ~LanguageHelper();
		virtual Compiler::OutputList preprocess(const Kiss::KarPtr &archive, const QStringList &searchPaths) const = 0;
	};
}

Q_DECLARE_INTERFACE(Kiss::LanguageHelper, "com.kipr.kiss.LanguageHelper/1.0");

#endif

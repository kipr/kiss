#ifndef _LANGUAGE_HELPER_HPP_
#define _LANGUAGE_HELPER_HPP_

#include <QObject>
#include <QStringList>

#include <kar.hpp>
#include <pcompiler/output.hpp>

namespace kiss
{
	class LanguageHelper : public QObject
	{
	Q_OBJECT
	public:
		virtual ~LanguageHelper();
		virtual Compiler::OutputList preprocess(const kiss::KarPtr &archive, const QStringList &searchPaths) const = 0;
	};
}

Q_DECLARE_INTERFACE(kiss::LanguageHelper, "com.kipr.kiss.LanguageHelper/1.0");

#endif

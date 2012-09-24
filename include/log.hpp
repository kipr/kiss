#ifndef _LOG_HPP_
#define _LOG_HPP_

#include "singleton.hpp"

#include <QString>

namespace Kiss
{
	class Log : public Singleton<Log>
	{
	public:
		void setLoggingLevel(const int& level);
		const int loggingLevel() const;

		enum Level {
			Errors,
			Warnings,
			Info,
			Debug
		};

		void error(const QString& message) const;
		void warning(const QString& message) const;
		void info(const QString& message) const;
		void debug(const QString& message) const;

		void message(const QString& message) const;
	private:

		int m_level;
	};
}

#endif

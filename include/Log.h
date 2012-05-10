#ifndef _LOG_H_
#define _LOG_H_

#include "Singleton.h"

#include <QString>

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

#endif

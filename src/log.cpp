#include "log.hpp"

#include <iostream>
#include <QTime>

#define ERROR_PREFIX QString("Error")
#define WARNING_PREFIX QString("Warning")
#define INFO_PREFIX QString("Info")
#define DEBUG_PREFIX QString("Debug")

using namespace Kiss;

void Log::setLoggingLevel(const int& level)
{
	m_level = level;
}

const int Log::loggingLevel() const
{
	return m_level;
}

void Log::error(const QString& message) const
{
	Log::message(ERROR_PREFIX + ": " + message);
}

void Log::warning(const QString& message) const
{
	Log::message(WARNING_PREFIX + ": " + message);
}

void Log::info(const QString& message) const
{
	Log::message(INFO_PREFIX + ": " + message);
}

void Log::debug(const QString& message) const
{
	Log::message(DEBUG_PREFIX + ": " + message);
}

void Log::message(const QString& message) const
{
	std::cout << "[" << QTime::currentTime().toString().toStdString() << "] " << message.toStdString() << std::endl;
}
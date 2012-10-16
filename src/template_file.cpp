#include "template_file.hpp"

#include <QDateTime>

#define KISS_LEXER "KISS_LEXER"
#define KISS_DATE "KISS_DATE"
#define END_KISS "END_KISS_META"

using namespace Kiss;
using namespace Kiss::Template;

File::File()
{
}

File::File(const QByteArray& data, const QString& lexer)
	: m_lexer(lexer), m_data(data)
{
}

bool File::isNull() const
{
	return m_data.isNull() || m_data.isEmpty();
}

bool File::hasLexer() const
{
	return !m_lexer.isEmpty();
}

const QString& File::lexer() const
{
	return m_lexer;
}

const QByteArray& File::data() const
{
	return m_data;
}

QString File::resolvedData() const
{
	QString dataString(m_data);
	dataString.replace("KISS_DATE", QDateTime::currentDateTime().toString("ddd MMMM d yyyy"));
	return dataString;
}

void File::setLexer(const QString& lexer)
{
	m_lexer = lexer;
}

void File::setData(const QByteArray& data)
{
	m_data = data;
}


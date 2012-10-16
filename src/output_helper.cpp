#include "output_helper.hpp"

#include <QString>
#include <QStringList>

using namespace Kiss;

Lines::Lines()
{
}

Lines::Lines(const LineList& warningLines, const LineList& errorLines)
	: m_warningLines(warningLines),
	m_errorLines(errorLines)
{
}

const Lines::LineList& Lines::warningLines() const
{
	return m_warningLines;
}

const Lines::LineList& Lines::errorLines() const
{
	return m_errorLines;
}

Lines Lines::operator+(const Lines& rhs) const
{
	return Lines(m_warningLines + rhs.warningLines(),
		m_errorLines + rhs.errorLines());
}

Lines OutputHelper::lines(const Compiler::Output& output)
{
	Lines::LineList errorLines;
	Lines::LineList warningLines;
	QString str = QString(output.error()) + "\n" + QString(output.output());
	QStringList lines = str.split("\n");
	
	foreach(const QString& line, lines) {
		const Lines::Type classification = classifyLine(line);
		if(classification == Lines::Unknown) continue;
		
		int lineNumber = line.section(":", 1, 1).toInt();
		if(--lineNumber < 0) continue;
		
		if(classification == Lines::Error) errorLines << lineNumber;
		else if(classification == Lines::Warning) warningLines << lineNumber;
	}
	
	return Lines(warningLines, errorLines);
}

Lines::Type OutputHelper::classifyLine(const QString& line)
{
	// Clang and GCC
	const QString gcc = line.section(":", 3, 3);
	if(gcc.contains("error", Qt::CaseInsensitive)) return Lines::Error;
	if(gcc.contains("warning", Qt::CaseInsensitive)) return Lines::Warning;
	
	// We hopefully didn't get here. Expand search.
	if(line.contains("error", Qt::CaseInsensitive)) return Lines::Error;
	if(line.contains("warning", Qt::CaseInsensitive)) return Lines::Warning;
	
	return Lines::Unknown;
}
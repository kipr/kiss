#ifndef _OUTPUT_HELPER_HPP_
#define _OUTPUT_HELPER_HPP_

#include "pcompiler/output.hpp"
#include <QList>

namespace Kiss
{
	class Lines
	{
	public:
		enum Type
		{
			Unknown,
			Warning,
			Error
		};
		
		typedef unsigned int line_t;
		typedef QList<line_t> LineList;
		
		Lines();
		Lines(const LineList& warningLines, const LineList& errorLines);
		
		const LineList& warningLines() const;
		const LineList& errorLines() const;
		
		Lines operator+(const Lines& rhs) const;
		
	private:
		QList<line_t> m_warningLines;
		QList<line_t> m_errorLines;
	};
	
	class OutputHelper
	{
	public:
		static Lines lines(const Compiler::Output& output);
	private:
		static Lines::Type classifyLine(const QString& line);
	};
}

#endif

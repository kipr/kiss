#include "LexerCPP.h"

#include "LexerFactory.h"

#include <QDebug>

using namespace Lexer;

CPP::CPP(const Constructor* constructor) : LexerBase(constructor)
{
}

QColor CPP::defaultColor(int style) const
{
    switch (style)
    {
	case Default:
		return QColor(0x80, 0x80, 0x80);

	case Comment:
	case CommentLine:
	case InactiveComment:
	case InactiveCommentLine:
		return QColor("green");

	case CommentDoc:
	case CommentLineDoc:
	case InactiveCommentDoc:
		return QColor(0x3f, 0x70, 0x3f);

	case Number:
		return QColor(0x00, 0x7f, 0x7f);

	case Keyword:
		return QColor("darkBlue");

	case DoubleQuotedString:
	case SingleQuotedString:
	case RawString:
		return QColor("darkRed");

	case PreProcessor:
		return QColor("darkBlue");

	case Operator:
	case UnclosedString:
		return QColor(0x00, 0x00, 0x00);

	case CommentDocKeyword:
		return QColor(0x30, 0x60, 0xa0);

	case CommentDocKeywordError:
		return QColor(0x80, 0x40, 0x20);

	case InactiveDefault:
	case InactiveUUID:
	case InactiveCommentLineDoc:
	case InactiveKeywordSet2:
	case InactiveCommentDocKeyword:
	case InactiveCommentDocKeywordError:
		return QColor(0xc0, 0xc0, 0xc0);


	case InactiveNumber:
		return QColor(0x90, 0xb0, 0x90);

	case InactiveKeyword:
		return QColor(0x90, 0x90, 0xb0);

	case InactiveDoubleQuotedString:
	case InactiveSingleQuotedString:
	case InactiveRawString:
		return QColor(0xb0, 0x90, 0xb0);

	case InactivePreProcessor:
		return QColor(0xb0, 0xb0, 0x90);

	case InactiveOperator:
	case InactiveIdentifier:
	case InactiveGlobalClass:
		return QColor(0xb0, 0xb0, 0xb0);

	case InactiveUnclosedString:
		return QColor(0x00, 0x00, 0x00);

	case InactiveVerbatimString:
		return QColor(0x00, 0x7f, 0x00);
    }

    return QsciLexerCPP::defaultColor(style);
}

QFont CPP::font(int style) const
{
	QFont f = QsciLexer::font(style);

	switch (style)
	{
	case Keyword:
	case InactiveKeyword:
	case Operator:
	case InactiveOperator:
		f.setBold(true);
		break;
	}


	return f;
}

QFont CPP::defaultFont(int style) const
{
	QFont f = QsciLexer::font(style);

	switch (style)
	{
	case Keyword:
	case InactiveKeyword:
	case Operator:
	case InactiveOperator:
		f.setBold(true);
		break;
	}


	return f;
}
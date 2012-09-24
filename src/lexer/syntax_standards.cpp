#include "syntax_standards.hpp"

QColor SyntaxStandards::defaultColor()
{
	return QColor(0x80, 0x80, 0x80);
}

QColor SyntaxStandards::commentColor()
{
	return QColor("green");
}

QColor SyntaxStandards::docColor()
{
	return QColor(0x3f, 0x70, 0x3f);
}

QColor SyntaxStandards::numberColor()
{
	return QColor(0x00, 0x7f, 0x7f);
}

QColor SyntaxStandards::keywordColor()
{
	return QColor("darkBlue");
}

QColor SyntaxStandards::stringColor()
{
	return QColor("darkRed");
}

QColor SyntaxStandards::preprocessorColor()
{
	return QColor("darkBlue");
}

QColor SyntaxStandards::operatorColor()
{
	return QColor(0x00, 0x00, 0x00);
}

QColor SyntaxStandards::unclosedStringColor()
{
	return QColor(0x00, 0x00, 0x00);
}

QColor SyntaxStandards::docKeywordColor()
{
	return QColor(0x30, 0x60, 0xa0);
}

QColor SyntaxStandards::docKeywordErrorColor()
{
	return QColor(0x80, 0x40, 0x20);
}

QColor SyntaxStandards::inactiveColor()
{
	return QColor(0xc0, 0xc0, 0xc0);
}

QColor SyntaxStandards::inactiveNumberColor()
{
	return QColor(0x90, 0xb0, 0x90);
}

QColor SyntaxStandards::inactiveKeywordColor()
{
	return QColor(0x90, 0x90, 0xb0);
}

QColor SyntaxStandards::inactiveStringColor()
{
	return QColor(0xb0, 0x90, 0xb0);
}

QColor SyntaxStandards::inactivePreprocessorColor()
{
	return QColor(0xb0, 0xb0, 0x90);
}

QColor SyntaxStandards::inactiveOperatorColor()
{
	return QColor(0xb0, 0xb0, 0xb0);
}

QColor SyntaxStandards::inactiveUnclosedStringColor()
{
	return QColor(0x00, 0x00, 0x00);
}

QColor SyntaxStandards::inactiveVerbatimStringColor()
{
	return QColor(0x00, 0x7f, 0x00);
}


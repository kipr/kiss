#ifndef __LEXER_SPEC_PROVIDER_H__
#define __LEXER_SPEC_PROVIDER_H__

#include <qplugin.h>
#include <QString>
#include "LexerSpec.h"

class LexerSpecProvider
{
public:
	LexerSpecProvider(const QString& extension) : m_extension(extension) {}
	
	virtual void init() = 0;
	QString extension() const { return m_extension; }
	
	LexerSpec* lexerSpec() { return &m_lexerSpec; }
	
protected:
	LexerSpec m_lexerSpec;
	QString m_extension;
};

Q_DECLARE_INTERFACE(LexerSpecProvider, "com.kipr.kiss-c.LexerSpecProvider/1.0");

#endif
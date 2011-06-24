#ifndef __LEXER_SPEC_MANAGER_H__
#define __LEXER_SPEC_MANAGER_H__

#include <QMap>
#include <QPluginLoader>
#include "LexerSpec.h"

class LexerSpecManager 
{
public:
	static LexerSpecManager& ref();
	
	LexerSpec* lexerSpec(const QString& ext);
private:	
	LexerSpecManager();
	~LexerSpecManager();
	
	void loadLexers();
	void loadLexer(const QString& fileName);
	
	QMap<QString, QPluginLoader*> m_lexers;
	
};

#endif
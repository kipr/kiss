#include "LexerFactory.h"

#include "LexerC.h"
#include "LexerCPP.h"
#include "LexerJava.h"

#include <QDebug>

using namespace Lexer;

Factory::Factory()
{
	registerLexerConstructor(new ConstructorJava(), QStringList() << "java");
	registerLexerConstructor(new ConstructorC(), QStringList() << "c");
	registerLexerConstructor(new ConstructorCPP(), QStringList() << "cpp" << "h" << "hpp");
}

Factory::~Factory()
{
	QList<Constructor*> removed;
	const QList<Constructor*>& l = m_constructors.values();
	foreach(Constructor* c, l) {
		if(removed.contains(c)) continue;
		
		delete c;
		removed.append(c);
	}
}

LexerBase* Factory::newLexer(const QString& name) const
{
	return constructor(name)->construct();
}

LexerBase* Factory::newLexerFromConstructor(const Constructor* constructor) const
{
	LexerBase* ret = constructor->_new();
	ret->setFont(m_font, -1);
	return ret;
}

Constructor* Factory::constructor(const QString& ext) const
{
	QMap<QString, Constructor*>::const_iterator i = m_constructors.find(ext);
	if(i == m_constructors.end()) return 0;
	
	return i.value();
}

void Factory::registerLexerConstructor(Constructor* c, const QStringList& exts)
{
	foreach(const QString& ext, exts) m_constructors[ext] = c;
}

void Factory::setAPIsForLexer(LexerBase* lexer, const QString& apis)
{
	if(!lexer) return;
	lexer->setAPIFile(apis);
}

bool Factory::isLexerFromConstructor(LexerBase* lexer, Constructor* constructor)
{
	if(!lexer) return false;
	return lexer->constructor() == constructor;
}

void Factory::setFont(const QFont& font)
{
	m_font = font;
}

QFont Factory::font() const
{
	return m_font;
}
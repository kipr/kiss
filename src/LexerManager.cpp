/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "LexerManager.h"
#include "Lexer.h"
#include "Kiss.h"
#include "JavaScript.h"

#include <QMap>

#include <QDir>
#include <QDebug>
#include <QFont>

#include "ErrorDialog.h"

#define DELIM " "

struct LexerScript : JavaScriptEnvironment
{	
	LexerSpec* lexerSpec(JavaScript* engine) const {
		QScriptValue global = engine->engine()->globalObject();
		LexerSpec* spec = new LexerSpec;

		QScriptValue exts = global.property("extensions");
		for(int i = 0; i < exts.property("length").toInt32(); ++i) {
			spec->extensions << exts.property(QString::number(i)).toString();
		}
		spec->language = global.property("language").toString();
		spec->lexer = global.property("lexer").toString();
		qWarning() << spec->language << spec->lexer;
		
		QScriptValue acwsObject = global.property("autoCompletionWordSeparators");
		for(int i = 0; i < acwsObject.property("length").toInt32(); ++i) {
			spec->autoCompletionWordSeparators << acwsObject.property(QString::number(i)).toString();
		}
		
		spec->blockEnd = global.property("blockEnd").toString();
		spec->blockEndStyle = global.property("blockEndStyle").toInt32();
		spec->blockStart = global.property("blockStart").toString();
		spec->blockStartStyle = global.property("blockStartStyle").toInt32();		
		spec->blockStartKeyword = global.property("blockStartKeyword").toString();
		spec->blockStartKeywordStyle = global.property("blockStartKeywordStyle").toInt32();

		spec->braceStyle = global.property("braceStyle").toInt32();
		spec->wordCharacters = global.property("wordCharacters").toString();

		QMap<QString, QString> defaultColor = JavaScript::objectStringMap(global.property("defaultColor"));
		for(QMap<QString, QString>::iterator it = defaultColor.begin(); it != defaultColor.end(); ++it) 
			spec->defaultColor[it.key().toInt()] = QColor(it.value());

		QScriptValue defaultEolFill = global.property("defaultEolFill");
		for(int i = 0; i < acwsObject.property("length").toInt32(); ++i)
			spec->defaultEolFill.append(defaultEolFill.property(QString::number(i)).toInt32());
		
		QMap<QString, QScriptValue> defaultFont = JavaScript::objectMap(global.property("defaultFont"));
		for(QMap<QString, QScriptValue>::iterator it = defaultFont.begin(); it != defaultFont.end(); ++it) {
			QMap<QString, QScriptValue> font = JavaScript::objectMap(it.value());
			spec->defaultFont[it.key().toInt()] = QFont(font["name"].toString(), font["size"].toInt32(),
				QFont::Bold, font["italic"].toBool());
		}
		
		QMap<QString, QString> defaultPaper = JavaScript::objectStringMap(global.property("defaultPaper"));
		for(QMap<QString, QString>::iterator it = defaultPaper.begin(); it != defaultPaper.end(); ++it) 
			spec->defaultPaper[it.key().toInt()] = QColor(it.value());
		
		QMap<QString, QString> keywords = JavaScript::objectStringMap(global.property("keywords"));
		for(QMap<QString, QString>::iterator it = keywords.begin(); it != keywords.end(); ++it) 
			spec->keywords[it.key().toInt()] = it.value();
		
		return spec;
	}
	
	void prepare(JavaScript* engine) {
		QScriptValue global = engine->engine()->globalObject();
		
		global.setProperty("Default", LexerCPP::Default);
		global.setProperty("Comment", LexerCPP::Comment);
		global.setProperty("CommentLine", LexerCPP::CommentLine);
		global.setProperty("CommentDoc", LexerCPP::CommentDoc);
		global.setProperty("Number", LexerCPP::Number);
		global.setProperty("Keyword", LexerCPP::Keyword);
		global.setProperty("String", LexerCPP::String);
		global.setProperty("Character", LexerCPP::Character);
		global.setProperty("UUID", LexerCPP::UUID);
		global.setProperty("PreProcessor", LexerCPP::PreProcessor);
		global.setProperty("Operator", LexerCPP::Operator);
		global.setProperty("Identifier", LexerCPP::Identifier);
		global.setProperty("StringEol", LexerCPP::StringEol);
		global.setProperty("Verbatim", LexerCPP::Verbatim);
		global.setProperty("Regex", LexerCPP::Regex);
		global.setProperty("CommentLineDoc", LexerCPP::CommentLineDoc);
		global.setProperty("Keyword2", LexerCPP::Keyword2);
		global.setProperty("CommentDocKeyword", LexerCPP::CommentDocKeyword);
		global.setProperty("CommentDocKeywordError", LexerCPP::CommentDocKeywordError);
		global.setProperty("GlobalClass", LexerCPP::GlobalClass);
		global.setProperty("StyleCount", LexerCPP::StyleCount);
		
		global.setProperty("Bold", QFont::Bold);
		global.setProperty("Light", QFont::Light);
		global.setProperty("Normal", QFont::Normal);
		
		global.setProperty("LexerCPP", LexerCPP::lexerName());
		
		engine->engine()->setGlobalObject(global);
	}
};

LexerManager::LexerManager() { loadLexers(); }
LexerManager::~LexerManager() { foreach(LexerSpec* spec, m_lexers) delete spec; }

LexerSpec* LexerManager::lexerSpec(const QString& ext) { return m_lexerMap.contains(ext) ? m_lexerMap[ext] : 0; }

void LexerManager::loadLexer(const QString& str)
{	
	LexerScript script;
	JavaScript engine(&script);
	QFile file(QDir(LEXER_FOLDER).filePath(str));
	QScriptValue ret = engine.evaluate(file);
	if(ret.isValid() && ret.isString()) {
		ErrorDialog::showError(0, "simple_error_with_action", QStringList() <<
			QObject::tr("Evaluation of ") + str + QObject::tr(" failed") << ret.toString() <<
			QObject::tr("Continuing without syntax highlighting support provided by ") + str);
		
		return;	
	}
	LexerSpec* spec = script.lexerSpec(&engine);
	if(!spec) return;
	
	foreach(const QString& ext, spec->extensions) m_lexerMap[ext] = spec;
	m_lexers.append(spec);
}

void LexerManager::loadLexers()
{
	foreach(const QString& str, QDir(LEXER_FOLDER).entryList(QStringList() << "*.js")) loadLexer(str);
}

QString LexerManager::getExpectedLocation(const QString&) const { return LEXER_FOLDER; }

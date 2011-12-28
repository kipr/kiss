#include "SourceFileShared.h"
#include "MainWindow.h"

const QPixmap& SourceFileShared::blackBullet() const 	{ return m_blackBullet; }
const QPixmap& SourceFileShared::blueBullet() const 	{ return m_blueBullet; }
const QPixmap& SourceFileShared::redBullet() const 	{ return m_redBullet; }
const QPixmap& SourceFileShared::yellowBullet() const 	{ return m_yellowBullet; }

const MacroString* SourceFileShared::templateMacro() const { return &m_templateMacro; }

Debugger* SourceFileShared::debugger() { return &m_debugger; }

SourceFileShared::SourceFileShared() :
	m_blackBullet(":/sourcefile/icon_set/icons/bullet_black.png"), 
	m_blueBullet(":/sourcefile/icon_set/icons/bullet_blue.png"), 
	m_redBullet(":/sourcefile/icon_set/icons/bullet_red.png"), 
	m_yellowBullet(":/sourcefile/icon_set/icons/bullet_yellow.png"),
	m_debugger(&MainWindow::ref())
{
	m_debugger.hide();

	m_templateMacro["KISS_DATE"] = new DateMacro();
}
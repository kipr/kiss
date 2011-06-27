#include "SourceFileShared.h"
#include "MainWindow.h"

SourceFileShared& SourceFileShared::ref()
{
	static SourceFileShared instance;
	return instance;
}

ChooseTargetDialog* SourceFileShared::chooseTargetDialog()
{
	return &m_chooseTargetDialog;
}

ChoosePortDialog* SourceFileShared::choosePortDialog()
{
	return &m_choosePortDialog;
}

void SourceFileShared::showFindDialog(SourceFile* sourceFile)
{
	m_findDialog.setSourceFile(sourceFile);
	m_findDialog.show();
}

void SourceFileShared::hideFindDialog()
{
	m_findDialog.setSourceFile(0);
	m_findDialog.hide();
}

const QPixmap& SourceFileShared::blackBullet() const
{
	return m_blackBullet;
}

const QPixmap& SourceFileShared::blueBullet() const
{
	return m_blueBullet;
}

const QPixmap& SourceFileShared::redBullet() const
{
	return m_redBullet;
}

const QPixmap& SourceFileShared::yellowBullet() const
{
	return m_yellowBullet;
}

SourceFileShared::SourceFileShared() : m_findDialog(&MainWindow::ref()),
	m_blackBullet(":/sourcefile/icon_set/icons/bullet_black.png"), 
	m_blueBullet(":/sourcefile/icon_set/icons/bullet_blue.png"), 
	m_redBullet(":/sourcefile/icon_set/icons/bullet_red.png"), 
	m_yellowBullet(":/sourcefile/icon_set/icons/bullet_yellow.png")
{
	m_chooseTargetDialog.hide();
	m_choosePortDialog.hide();
	m_findDialog.hide();
}

SourceFileShared::SourceFileShared(const SourceFileShared& rhs) {
	Q_UNUSED(rhs);
}
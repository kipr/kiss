#ifndef __SOURCEFILESHARED_H__
#define __SOURCEFILESHARED_H__

#include <QPixmap>
#include "ChooseTargetDialog.h"
#include "ChoosePortDialog.h"
#include "FindDialog.h"

class SourceFileShared
{
public:
	static SourceFileShared& ref();
	
	ChooseTargetDialog* chooseTargetDialog();
	ChoosePortDialog* choosePortDialog();
	
	void showFindDialog(SourceFile* sourceFile);
	void hideFindDialog();
	
	const QPixmap& blackBullet() const;
	const QPixmap& blueBullet() const;
	const QPixmap& redBullet() const; 
	const QPixmap& yellowBullet() const;
private:
	SourceFileShared();
	SourceFileShared(const SourceFileShared& rhs);
	
	ChooseTargetDialog m_chooseTargetDialog;
	ChoosePortDialog m_choosePortDialog;
	FindDialog m_findDialog;
	
	QPixmap m_blackBullet;
	QPixmap m_blueBullet;
	QPixmap m_redBullet;
	QPixmap m_yellowBullet;

};

#endif

#ifndef _SOURCEFILESHARED_H_
#define _SOURCEFILESHARED_H_

#include "Debugger.h"
#include "Singleton.h"

#include <QPixmap>

class SourceFileShared : public Singleton<SourceFileShared>
{
public:	
	SourceFileShared();
	
	const QPixmap& blackBullet() const;
	const QPixmap& blueBullet() const;
	const QPixmap& redBullet() const; 
	const QPixmap& yellowBullet() const;
	Debugger* debugger();
private:
	QPixmap m_blackBullet;
	QPixmap m_blueBullet;
	QPixmap m_redBullet;
	QPixmap m_yellowBullet;
	
	Debugger m_debugger;
};

#endif

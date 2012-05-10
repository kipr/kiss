#ifndef _LOGWINDOW_H_
#define _LOGWINDOW_H_

#include <QWidget>

#include "ui_LogWindow.h"
#include "Singleton.h"

#include "LogStreamBuf.h"

class LogWindow : public QWidget, private Ui::LogWindow, public Singleton<LogWindow>
{
public:
	LogWindow();
	
	friend class LogStreamBuf;
private:
	void append(const QString& str);
};

#endif

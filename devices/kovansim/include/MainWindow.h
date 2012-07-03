#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include "Kovan.h"
#include "Server.h"
#include "Discovery.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
	MainWindow();
	
private:
	Kovan m_kovan;
	Server m_server;
	Discovery m_discovery;
};

#endif

#include "MainWindow.h"

#include <QDebug>

MainWindow::MainWindow() : m_kovan(), m_server(&m_kovan), m_discovery(this)
{
	if(!m_discovery.setup()) {
		qDebug() << "Failed to setup ohaiyo listener";
	}
	if(!m_server.listen(QHostAddress::Any, 8075)) {
		qDebug() << "Failed to listen";
	}
}
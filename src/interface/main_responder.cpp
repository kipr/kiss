#include "main_responder.hpp"
#include "main_window.hpp"

#include <QDebug>

using namespace Kiss;
using namespace Kiss::Target;

MainResponder::MainResponder(Widget::MainWindow *mainWindow)
	: m_mainWindow(mainWindow)
{
}

MainResponder::~MainResponder()
{
	
}

void MainResponder::response(Target *target, const Response &response)
{
	qDebug() << response.type();
	if(response.type() == "connection" && !response.data().toBool()) {
		m_mainWindow->setStatusMessage(QObject::tr("Connection Failed!"), 5000);
		return; 
	} else if(response.type() == "compile") {
		if(response.data().canConvert<bool>()) {
			m_mainWindow->setStatusMessage(response.data().toBool()
				? QObject::tr("Compile Succeeded!")
				: QObject::tr("Compile Failed!"), 5000);
		} else if(response.data().canConvert<Compiler::OutputList>()) {
			m_mainWindow->setOutputList(response.data().value<Compiler::OutputList>());
		}
	}
}

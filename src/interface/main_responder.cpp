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

// TODO: Create defines or something for all of these keys
void MainResponder::response(Target *target, const Response &response)
{
	qDebug() << response.type();
	if(response.type() == "connection" && !response.data().toBool()) {
		m_mainWindow->setStatusMessage(QObject::tr("Connection Failed!"), 5000);
		return; 
	} else if(response.type() == "compile") {
		bool success = false;
		if(response.data().canConvert<bool>()) {
			success = response.data().toBool();
		} else if(response.data().canConvert<Compiler::OutputList>()) {
			Compiler::OutputList output = response.data().value<Compiler::OutputList>();
			m_mainWindow->setOutputList(output);
			success = Compiler::Output::isSuccess(output);
		}
		m_mainWindow->setStatusMessage(success
			? QObject::tr("Compile Succeeded!")
			: QObject::tr("Compile Failed!"), 5000);
	} else if(response.type() == "download") {
		bool success = response.data().toBool();
		m_mainWindow->setStatusMessage(success
			? QObject::tr("Download Succeeded!")
			: QObject::tr("Download Failed!"), 5000);
	} else if(response.type() == "run") {
		bool success = response.data().toBool();
		m_mainWindow->setStatusMessage(success
			? QObject::tr("Run Succeeded!")
			: QObject::tr("Run Failed!"), 5000);
	} else if(response.type() == "began_compile") {
		m_mainWindow->setStatusMessage(QObject::tr("Compiling..."));
	} else if(response.type() == "began_download") {
		m_mainWindow->setStatusMessage(QObject::tr("Downloading..."));
	} else if(response.type() == "began_run") {
		m_mainWindow->setStatusMessage(QObject::tr("Running..."));
	}
}

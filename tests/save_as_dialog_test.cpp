#include <QApplication>
#include <QDebug>

#include "save_as_dialog.hpp"

using namespace kiss;
using namespace kiss::dialog;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	SaveAs dialog;
	dialog.setNameFilters(QStringList() << "*.cpp");
	dialog.setFileName("test.c");
	dialog.setRootPath("/Users/beta/Desktop");
	dialog.exec();
	
	QString filePath = dialog.filePath();
	qDebug() << filePath;
	
	return 0;
}
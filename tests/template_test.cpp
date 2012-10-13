#include "template_file.hpp"
#include "template_pack.hpp"
#include "template_manager.hpp"
#include "template_model.hpp"
#include "template_dialog.hpp"

#include <QApplication>
#include <QDebug>

#define LOCATION "/Users/beta/Desktop/test.pack"

using namespace Kiss;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QString testTemplate = "// Created on KISS_DATE\n\n"
	"int main(int argc, char *argv[]) {\n"
	"\treturn 0;\n"
	"}";
	
	Template::File basicTemplate(testTemplate.toLatin1(), "c");
	
	Template::PackPtr pack = Template::Pack::create();
	pack->addFile("Basic", basicTemplate);
	pack->setName("C/C++");
	pack->save(LOCATION);
	
	pack = Template::Pack::load(LOCATION);
	if(!pack) return 1;
	
	qDebug() << "Templates:" << pack->templates();
	qDebug() << "Data for Basic:" << pack->file("Basic.c").lexer();
	
	Template::Manager manager;
	manager.addPack(pack);
	
	Dialog::Template dialog(&manager);
	dialog.exec();
	
	return app.exec();
}
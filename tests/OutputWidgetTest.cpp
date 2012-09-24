#include "OutputWidget.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	OutputWidget outputWidget;
	outputWidget.resize(800, 300);
	outputWidget.show();
	
	Compiler::Output output;
	output.setExitCode(0);
	output.setError(QString("m.cpp: In function 'int main()':"
	"m.cpp:6:12: error: 'void X<A, 2>::f() [with int A = 1]' is private"
	"m.cpp:10:19: error: within this context"
	"m.cpp:2:12: error: 'void X<A, B>::f() [with int A = 2, int B = 1]' is private"
	"m.cpp:11:19: error: within this context").toAscii());
	
	Compiler::OutputList list;
	list << output;

	outputWidget.setOutputList(list);
	
	return app.exec();
}
#include <QApplication>
#include "output_widget.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	OutputWidget outputWidget;
	outputWidget.resize(800, 300);
	outputWidget.show();
	outputWidget.raise();
	
	Compiler::Output output;
	output.setExitCode(0);
	output.setError(QString("m.cpp: In function 'int main()':\n"
	"m.cpp:6:12: error: 'void X<A, 2>::f() [with int A = 1]' is private\n"
	"m.cpp:10:19: error: within this context\n"
	"m.cpp:2:12: error: 'void X<A, B>::f() [with int A = 2, int B = 1]' is private\n"
	"m.cpp:11:19: error: within this context\n").toAscii());
	
	Compiler::Output output2;
	output2.setExitCode(0);
	output2.setError(QString(
	"/Volumes/Eve/Development/kipr/kiss/src/widgets/output_widget.cpp:85:53: error: no member named 'toList' in 'QList<Compiler::Output>'\n"
	"        foreach(const Compiler::Output& output, outputList.toList()) {\n"
	"        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~\n"
	"/Library/Frameworks/QtCore.framework/Headers/qglobal.h:2258:35: note: expanded from macro 'Q_FOREACH'\n"
	"for (QForeachContainer<__typeof__(container)> _container_(container); \\\n"
	"                                  ^\n"
	"/Volumes/Eve/Development/kipr/kiss/src/widgets/output_widget.cpp:85:2: error: C++ requires a type specifier for all declarations\n"
	"        foreach(const Compiler::Output& output, outputList.toList()) {\n"
	"        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	"/Library/Frameworks/QtCore.framework/Headers/qglobal.h:2325:21: note: expanded from macro 'foreach'\n"
	"#    define foreach Q_FOREACH\n"
	"                    ^\n"
	"/Library/Frameworks/QtCore.framework/Headers/qglobal.h:2258:47: note: expanded from macro 'Q_FOREACH'\n"
	"for (QForeachContainer<__typeof__(container)> _container_(container); \\\n"
	"                                              ^~~~~~~~~~~\n"
	"/Volumes/Eve/Development/kipr/kiss/src/widgets/output_widget.cpp:85:53: error: no member named 'toList' in 'QList<Compiler::Output>'\n"
	"        foreach(const Compiler::Output& output, outputList.toList()) {\n"
	"        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~\n"
	"/Library/Frameworks/QtCore.framework/Headers/qglobal.h:2258:59: note: expanded from macro 'Q_FOREACH'\n"
	"for (QForeachContainer<__typeof__(container)> _container_(container); \\\n"
	"                                                          ^\n"
	"3 errors generated.\n").toAscii());
	
	Compiler::OutputList list;
	list << output << output2;

	outputWidget.setOutputList(list);
	
	return app.exec();
}
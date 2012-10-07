#include <QApplication>
#include <QFont>

#include "c_highlighter.hpp"
#include "code_editor.hpp"

using namespace Kiss;
using namespace Kiss::Widget;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	CodeEditor editor;
	editor.setEditorFont(QFont("Monaco", 24));
	
	new CHighlighter(editor.document());
	MarginIndicator error(QColor(255, 0, 0, 70));
	MarginIndicator breakpoint(QColor(0, 0, 255, 70));
	editor.setMarginIndicator(10, error);
	editor.setMarginIndicator(5, breakpoint);
	editor.show();
	editor.raise();
	
	return app.exec();
}
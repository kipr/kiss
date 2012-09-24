#include "log_window.hpp"

using namespace Kiss::Widget;

LogWindow::LogWindow()
	: QWidget(0, Qt::SubWindow)
{
	setupUi(this);
}

void LogWindow::append(const QString& str)
{
	QTextCursor cursor(ui_log->textCursor());
	cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
	ui_log->setTextCursor(cursor);
	ui_log->insertPlainText(str);
}
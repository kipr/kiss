#include "ErrorDialog.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QFile>

ErrorDialog::ErrorDialog(QWidget* widget) : QDialog(widget) { setupUi(this); }

void ErrorDialog::setMessage(const QString& error, const QStringList& args)
{
	ui_error->clear();
	
	QString message = QString("Error template ") + error + " does not exist.";
	QFile file(QString(":/errors/") + error + ".txt");
	if(file.open(QIODevice::ReadOnly)) {
		message = file.readAll();
	}
	for(int i = 0; i < args.size(); ++i) {
		message = message.replace(QString("${") + QString::number(i + 1) + "}", args.at(i));
	}
	ui_error->setText(message);
}

void ErrorDialog::on_ui_copy_clicked()
{
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->setText(ui_error->toPlainText());
}

void ErrorDialog::on_ui_error_anchorClicked(const QUrl& link) { QDesktopServices::openUrl(link); }

void ErrorDialog::showError(QWidget* parent, const QString& error, const QStringList& args)
{
	ErrorDialog dialog(parent);
	dialog.setMessage(error, args);
	dialog.exec();
}
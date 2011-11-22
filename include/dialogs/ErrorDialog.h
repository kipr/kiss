#ifndef _ERRORDIALOG_H_
#define _ERRORDIALOG_H_

#include "ui_ErrorDialog.h"

class ErrorDialog : public QDialog, public Ui::ErrorDialog
{
	Q_OBJECT
public:
	ErrorDialog(QWidget* parent = 0);
	
	void setMessage(const QString& error, const QStringList& args = QStringList());
	
	static void showError(QWidget* parent, const QString& error, const QStringList& args = QStringList());
private slots:
	void on_ui_error_anchorClicked(const QUrl& link);
	void on_ui_copy_clicked();
};

#endif

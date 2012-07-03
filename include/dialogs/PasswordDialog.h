#ifndef _PASSWORDDIALOG_H_
#define _PASSWORDDIALOG_H_

#include <QDialog>

namespace Ui
{
	class PasswordDialog;
}

class PasswordDialog : public QDialog
{
Q_OBJECT
public:
	PasswordDialog(QWidget *parent = 0);
	~PasswordDialog();
	
	QByteArray hash() const; // Get SHA1 Hash of password
private:
	Ui::PasswordDialog *ui;
};

#endif

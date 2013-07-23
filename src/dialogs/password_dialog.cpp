#include "password_dialog.hpp"
#include "ui_PasswordDialog.h"

#include <QCryptographicHash>

using namespace kiss::dialog;

Password::Password(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::PasswordDialog();
	ui->setupUi(this);
}

Password::~Password()
{
	delete ui;
}

int Password::exec()
{
	ui->password->setFocus();
	if(!QDialog::exec()) return QDialog::Rejected;
	return QDialog::Accepted;
}

QString Password::password() const
{
	return ui->password->text();
}

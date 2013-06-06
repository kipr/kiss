#include "password_dialog.hpp"
#include "ui_PasswordDialog.h"

#include <QCryptographicHash>

using namespace Kiss::Dialog;

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

QString Password::password() const
{
	return ui->password->text();
}
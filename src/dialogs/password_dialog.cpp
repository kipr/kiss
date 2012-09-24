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

QByteArray Password::hash() const
{
	QCryptographicHash hash(QCryptographicHash::Sha1);
	hash.addData(ui->password->text().toUtf8());
	return hash.result();
}
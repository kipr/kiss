#include "PasswordDialog.h"
#include "ui_PasswordDialog.h"

#include <QCryptographicHash>

PasswordDialog::PasswordDialog(QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::PasswordDialog();
	ui->setupUi(this);
}

PasswordDialog::~PasswordDialog()
{
	delete ui;
}

QByteArray PasswordDialog::hash() const
{
	QCryptographicHash hash(QCryptographicHash::Sha1);
	hash.addData(ui->password->text().toUtf8());
	return hash.result();
}
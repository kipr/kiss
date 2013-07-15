#include "add_to_project_dialog.hpp"
#include "ui_AddToProjectDialog.h"

AddToProjectDialog::AddToProjectDialog(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
}

AddToProjectDialog::~AddToProjectDialog()
{
}

AddToProjectDialog::AddType AddToProjectDialog::type() const
{
	if(ui_moveButton->isChecked()) return Move;
	else if(ui_copyButton->isChecked()) return Copy;
	else if(ui_absRefButton->isChecked()) return AbsoluteReference;
	else return RelativeReference;
}
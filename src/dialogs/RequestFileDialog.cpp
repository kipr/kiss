#include "RequestFileDialog.h"

RequestFileDialog::RequestFileDialog(Target* target) : m_target(target) { setupUi(this); }

int RequestFileDialog::exec()
{
	ui_files->clear();
	ui_files->addItems(m_target->requestDir(m_target->requestFilePath()));
	return QDialog::exec();
}

QString RequestFileDialog::selectedFile() { return ui_files->currentItem()->text(); }
#include "compiler_flag_dialog.hpp"

#include <pcompiler/compilers.hpp>
#include <pcompiler/base.hpp>

using namespace kiss;
using namespace kiss::dialog;

CompilerFlagDialog::CompilerFlagDialog(QWidget *parent, const QString &flag, const QString &value) :
	QDialog(parent)
{
	setupUi(this);
	
	QStringList flags;
	QList<const Compiler::Base *> compilers = Compiler::Compilers::instance()->compilers();
	foreach(const Compiler::Base *compiler, compilers) flags << compiler->flags();
	flags.removeDuplicates();
	ui_boxFlag->addItems(flags);
	
	if(flag.isNull() && value.isNull()) return;
	
	ui_boxFlag->setCurrentIndex(ui_boxFlag->findText(flag));
	ui_lineEditValue->setText(value);
}

CompilerFlagDialog::~CompilerFlagDialog()
{
}

const QString CompilerFlagDialog::flag() const
{
	return ui_boxFlag->currentText();
}

const QString CompilerFlagDialog::value() const
{
	return ui_lineEditValue->text();
}
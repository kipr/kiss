#ifndef _COMPILER_FLAG_DIALOG_HPP_
#define _COMPILER_FLAG_DIALOG_HPP_

#include <QDialog>

#include "ui_CompilerFlagDialog.h"

namespace kiss
{
	namespace dialog
	{
		class CompilerFlagDialog : public QDialog, private Ui::CompilerFlagDialog
		{
			Q_OBJECT
				
		public:
			CompilerFlagDialog(QWidget *parent, const QString &flag = QString(), const QString &value = QString());
			~CompilerFlagDialog();
			
			const QString flag() const;
			const QString value() const;
		};
	}
}

#endif
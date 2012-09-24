#ifndef _PASSWORDDIALOG_H_
#define _PASSWORDDIALOG_H_

#include <QDialog>

namespace Ui
{
	class PasswordDialog;
}

namespace Kiss
{
	namespace Dialog
	{
		class Password : public QDialog
		{
		Q_OBJECT
		public:
			Password(QWidget *parent = 0);
			~Password();

			QByteArray hash() const; // Get SHA1 Hash of password
		private:
			Ui::PasswordDialog *ui;
		};
	}
}

#endif

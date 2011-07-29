#ifndef __REQUESTFILEDIALOG_H__
#define __REQUESTFILEDIALOG_H__

#include <QDialog>
#include "ui_RequestFileDialog.h"
#include "Target.h"

class RequestFileDialog : public QDialog, public Ui::RequestFileDialog
{
public:
	RequestFileDialog(Target* target);
	int exec();
	QString selectedFile();
private:
	Target* m_target;
};

#endif

#ifndef __SOURCEDIALOG_H__
#define __SOURCEDIALOG_H__

#include "ui_SourceDialog.h"

#include <QDialog>

class SourceDialog : public QDialog, private Ui::SourceDialog
{
public:
	SourceDialog(QWidget* parent = 0);
	
	void setSource(const QString& source);
	QString source();
};

#endif

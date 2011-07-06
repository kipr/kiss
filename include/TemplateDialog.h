#ifndef __TEMPLATEDIALOG_H__
#define __TEMPLATEDIALOG_H__

#include "ui_TemplateDialog.h"
#include <QDialog>
#include <QFile>

class TemplateDialog : public QDialog, private Ui::TemplateDialog
{
	Q_OBJECT
public:
	TemplateDialog(QWidget* parent = 0);
	
	int exec();
	int execTarget();
	
	// Returns to the path of the target file for the selected target
	QString selectedTargetFilePath();
	QString templateFile();
private slots:
	void on_ui_targets_currentItemChanged(QListWidgetItem* current, QListWidgetItem* prev);	
};

#endif

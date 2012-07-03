#ifndef _NEWPROJECTWIZARD_H_
#define _NEWPROJECTWIZARD_H_

#include <QDialog>
#include <QList>

#include "ui_NewProjectWizard.h"

class NewProjectWizard : public QDialog, private Ui::NewProjectWizard
{
Q_OBJECT
public:
	NewProjectWizard(QWidget* parent = 0);
	
	void setTargetPlatformEnabled(bool enabled);
	
	QString projectName() const;
	QString saveLocation() const;
	QString targetPlatform() const;
	
public slots:
	void on_ui_browse_clicked();
	void on_ui_projectName_textChanged(const QString& text);
private:
	void updateSaveLocation();
	
	QString m_savePath;
};

#endif

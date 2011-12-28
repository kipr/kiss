#ifndef _SOURCELOCALFAILED_H_
#define _SOURCELOCALFAILED_H_

#include <QWidget>
#include "ui_SourceLocalFailed.h"

class SourceFile;

class SourceLocalFailed : public QWidget, public Ui::SourceLocalFailed
{
Q_OBJECT
public:
	SourceLocalFailed(QWidget* parent);
	
	void performAction(const QString filePath);
	
	void setSourceFile(SourceFile* sourceFile);
	
	void setState(int state);
	int state();
	
	enum {
		Always,
		Ask,
		Never
	};
private slots:
	void on_ui_always_clicked();
	void on_ui_yes_clicked();
	void on_ui_no_clicked();
	void on_ui_never_clicked();

private:
	SourceFile* m_sourceFile;
	int m_state;
	
	QString m_filePath;
};

#endif

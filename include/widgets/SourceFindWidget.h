#ifndef _SOURCEFINDWIDGET_H_
#define _SOURCEFINDWIDGET_H_

#include <QWidget>
#include "ui_SourceFindWidget.h"

class SourceFile;

class SourceFindWidget : public QWidget, public Ui::SourceFindWidget
{
Q_OBJECT
public:
	SourceFindWidget(QWidget* parent);
	
	void setSourceFile(SourceFile* sourceFile);
	
	void setModified(bool m);
	bool isModified();

public slots:
	void show();
	
private slots:
	void on_ui_next_clicked();
	void on_ui_find_textChanged(const QString& text);
	void on_ui_matchCase_stateChanged(int state);
	void on_ui_replaceNext_clicked();
	void on_ui_replaceAll_clicked();
	
private:
	SourceFile* m_sourceFile;
	
	bool m_findModified;
};

#endif

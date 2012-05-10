#ifndef _ERRORWIDGET_H_
#define _ERRORWIDGET_H_

#include "ui_ErrorWidget.h"
#include "Compiler.h"
#include "WorkingUnit.h"

class ErrorWidget : public QWidget, private Ui::ErrorWidget
{
public:
	ErrorWidget(QWidget* parent = 0);
	
	void setCompileResults(WorkingUnit* unit, const CompileResults& results);
public slots:
	void workingUnitChanged(WorkingUnit* unit);
private slots:
	void on_ui_viewMode_currentIndexChanged(int index);
private:
	void setCategorized();
	void setRaw();
	WorkingUnit* m_unit;
};

#endif

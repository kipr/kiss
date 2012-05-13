#ifndef _ERRORWIDGET_H_
#define _ERRORWIDGET_H_

#include "ui_ErrorWidget.h"
#include "Compiler.h"
#include "WorkingUnit.h"

#define DEFAULT_ERROR_KEY "Errors"
#define DEFAULT_WARNING_KEY "Warnings"
#define DEFAULT_LINKER_KEY "Linker"

class ErrorWidget : public QWidget, private Ui::ErrorWidget
{
Q_OBJECT
public:
	ErrorWidget(QWidget* parent = 0);
	
	void setCompileResult(const WorkingUnit* unit, const CompileResult& results);
public slots:
	void workingUnitChanged(const WorkingUnit* current);
private slots:
	void on_ui_viewMode_currentIndexChanged(int index);
private:
	void setCategorized();
	void setRaw();
	const WorkingUnit* m_current;
	const WorkingUnit* m_unit;
};

#endif

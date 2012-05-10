#include "ErrorWidget.h"

#include <QListWidget>
#include <QDebug>

ErrorWidget::ErrorWidget(QWidget* parent = 0) : QWidget(parent), m_unit(0) { setupUi(this); }

void ErrorWidget::setCompileResults(WorkingUnit* unit, const CompileResults& results)
{
	m_unit = unit;
	
	while(ui_tabs->count()) {
		QWidget* widget = ui_tabs->widget(0);
		ui_tabs->removeTab(0);
		delete widget;
	}
	
	const QMap<QString, QStringList>& categories = results.categorizedOutput();
	foreach(const QString& key, categories.keys()) {
		QListWidget* listWidget = new QListWidget(this);
		foreach(const QString& line, categories.value(key)) {
			listWidget->addItem(line);
		}
		ui_tabs->addTab(listWidget, key);
	}
	
	ui_raw->setText("...");
}

void ErrorWidget::workingUnitChanged(WorkingUnit* unit)
{
	if(unit == m_unit) show();
}

void ErrorWidget::on_ui_viewMode_currentIndexChanged(int index)
{
	switch(index) {
		case 0: setCategorized(); break;
		case 1: setRaw(); break;
		default: qWarning() << "Error View set to unrecognized view mode"; break;
	}
}

void ErrorWidget::setCategorized()
{
	ui_raw->hide();
	ui_tabs->show();
}

void ErrorWidget::setRaw()
{
	ui_raw->show();
	ui_tabs->hide();
}
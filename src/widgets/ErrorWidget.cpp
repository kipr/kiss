#include "ErrorWidget.h"

#include <QListWidget>
#include <QDebug>

ErrorWidget::ErrorWidget(QWidget* parent) : QWidget(parent), m_unit(0)
{
	setupUi(this);
	setCategorized();
}

void ErrorWidget::setCompileResult(const WorkingUnit* unit, const CompileResult& results)
{
	m_unit = unit;
	if(m_current == m_unit) show();
	
	while(ui_tabs->count()) {
		QWidget* widget = ui_tabs->widget(0);
		ui_tabs->removeTab(0);
		delete widget;
	}
	
	const QMap<QString, QStringList>& categories = results.categorizedOutput();
	int largest = -1;
	int largestSize = 0;
	foreach(const QString& key, categories.keys()) {
		QListWidget* listWidget = new QListWidget(this);
		const QStringList& items = categories.value(key);
		foreach(const QString& line, items) listWidget->addItem(line);
		const int i = ui_tabs->addTab(listWidget, key);
		if(items.size() > largestSize && largestSize >= 0) largest = i;
		if(key == DEFAULT_ERROR_KEY) {
			largest = i;
			largestSize = -1;
		}
	}
	
	if(largest >= 0) ui_tabs->setCurrentIndex(largest);
	
	ui_raw->setText(results.raw());
	
	if(!ui_tabs->count()) hide();
}

void ErrorWidget::workingUnitChanged(const WorkingUnit* current)
{
	m_current = current;
	setVisible(m_current == m_unit && ui_tabs->count());
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
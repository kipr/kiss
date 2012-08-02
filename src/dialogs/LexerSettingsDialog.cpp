#include "LexerSettingsDialog.h"
#include "ColorBox.h"

#include <QDebug>

LexerSettingsDialog::LexerSettingsDialog(QMap<QString, QColor> lexerSettings, QWidget *parent)
	: QDialog(parent), m_lexerSettings(lexerSettings)
{
    setupUi(this);
    
    ui_table->setColumnWidth(0, 170);
    for(int i = 0; i < ui_table->rowCount(); ++i) {
    	ColorBox *box = new ColorBox(ui_table);
    	box->setProperty("row", i);
    	connect(box, SIGNAL(colorChanged(QColor)), SLOT(settingChanged(QColor)));
    	ui_table->setCellWidget(i, 1, box);
    }
}

LexerSettingsDialog::~LexerSettingsDialog() {}

QMap<QString, QColor> LexerSettingsDialog::settings()
{
	return m_lexerSettings;
}

void LexerSettingsDialog::settingChanged(QColor color)
{
	ColorBox *box = (ColorBox *)sender();
	QString setting = ui_table->item(box->property("row").toInt(), 0)->text();
	m_lexerSettings.insert(setting, color);
}

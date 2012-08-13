#include "ThemeSettingsDialog.h"
#include "ui_ThemeSettingsDialog.h"
#include "SyntaxStandards.h"

#include <QDebug>

ThemeSettingsDialog::ThemeSettingsDialog(QMap<QString, QColor> lexerSettings, QColor backgroundColor, QFont font, int fontSize, QWidget *parent)
	: QDialog(parent), m_lexerSettings(lexerSettings)
{
	ui = new Ui::ThemeSettingsDialog();
	ui->setupUi(this);
	
	ui->ui_table->setColumnWidth(0, 180);
	
	const int rows = ui->ui_table->rowCount();
	boxes = new ColorBox *[rows];
	for(int i = 0; i < rows; ++i) {
		boxes[i] = new ColorBox(ui->ui_table);
		boxes[i]->setProperty("row", i);
		connect(boxes[i], SIGNAL(colorChanged(QColor)), SLOT(settingChanged(QColor)));
		ui->ui_table->setCellWidget(i, 1, boxes[i]);
	}
	
	boxes[0]->setColor(m_lexerSettings.value(DEFAULT, SyntaxStandards::defaultColor()));
	boxes[1]->setColor(m_lexerSettings.value(COMMENT, SyntaxStandards::commentColor()));
	boxes[2]->setColor(m_lexerSettings.value(DOC_COMMENT, SyntaxStandards::docColor()));
	boxes[3]->setColor(m_lexerSettings.value(NUMBER, SyntaxStandards::numberColor()));
	boxes[4]->setColor(m_lexerSettings.value(KEYWORD, SyntaxStandards::keywordColor()));
	boxes[5]->setColor(m_lexerSettings.value(STRING, SyntaxStandards::stringColor()));
	boxes[6]->setColor(m_lexerSettings.value(PREPROCESSOR, SyntaxStandards::preprocessorColor()));
	boxes[7]->setColor(m_lexerSettings.value(OPERATOR, SyntaxStandards::operatorColor()));
	boxes[8]->setColor(m_lexerSettings.value(UNCLOSED_STRING, SyntaxStandards::unclosedStringColor()));
	boxes[9]->setColor(m_lexerSettings.value(DOC_KEYWORD, SyntaxStandards::docKeywordColor()));
	boxes[10]->setColor(m_lexerSettings.value(DOC_KEYWORD_ERROR, SyntaxStandards::docKeywordErrorColor()));
	
	ui->ui_backgroundColorBox->setColor(backgroundColor);
	ui->ui_fontBox->setCurrentFont(font);
	ui->ui_fontSizeBox->setValue(fontSize);
}

ThemeSettingsDialog::~ThemeSettingsDialog()
{
	for(int i = 0; i < ui->ui_table->rowCount(); ++i) delete boxes[i];
	delete boxes;
	delete ui;
}

QMap<QString, QColor> ThemeSettingsDialog::settings()
{
	return m_lexerSettings;
}

QColor ThemeSettingsDialog::backgroundColor()
{
	return ui->ui_backgroundColorBox->color();
}

QFont ThemeSettingsDialog::font()
{
	return ui->ui_fontBox->currentFont();
}

int ThemeSettingsDialog::fontSize()
{
	return ui->ui_fontSizeBox->value();
}

void ThemeSettingsDialog::settingChanged(QColor color)
{
	ColorBox *box = (ColorBox *)sender();
	QString setting = ui->ui_table->item(box->property("row").toInt(), 0)->text();
	m_lexerSettings.insert(setting, color);
}

#include "LexerSettingsDialog.h"
#include "ui_LexerSettingsDialog.h"
#include "SyntaxStandards.h"

#include <QDebug>

LexerSettingsDialog::LexerSettingsDialog(QMap<QString, QColor> lexerSettings, QWidget *parent)
	: QDialog(parent), m_lexerSettings(lexerSettings)
{
	ui = new Ui::LexerSettingsDialog();
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
}

LexerSettingsDialog::~LexerSettingsDialog()
{
	for(int i = 0; i < ui->ui_table->rowCount(); ++i) delete boxes[i];
	delete boxes;
	delete ui;
}

QMap<QString, QColor> LexerSettingsDialog::settings()
{
	return m_lexerSettings;
}

void LexerSettingsDialog::on_ui_buttonBox_clicked(QAbstractButton *button)
{
	if((QPushButton *)button == ui->ui_buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
		boxes[0]->setColor(SyntaxStandards::defaultColor());
		boxes[1]->setColor(SyntaxStandards::commentColor());
		boxes[2]->setColor(SyntaxStandards::docColor());
		boxes[3]->setColor(SyntaxStandards::numberColor());
		boxes[4]->setColor(SyntaxStandards::keywordColor());
		boxes[5]->setColor(SyntaxStandards::stringColor());
		boxes[6]->setColor(SyntaxStandards::preprocessorColor());
		boxes[7]->setColor(SyntaxStandards::operatorColor());
		boxes[8]->setColor(SyntaxStandards::unclosedStringColor());
		boxes[9]->setColor(SyntaxStandards::docKeywordColor());
		boxes[10]->setColor(SyntaxStandards::docKeywordErrorColor());
	}
}

void LexerSettingsDialog::settingChanged(QColor color)
{
	ColorBox *box = (ColorBox *)sender();
	QString setting = ui->ui_table->item(box->property("row").toInt(), 0)->text();
	m_lexerSettings.insert(setting, color);
}

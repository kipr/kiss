/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "theme_settings_dialog.hpp"
#include "ui_ThemeSettingsDialog.h"

#include "editor_settings_dialog.hpp"
#include "syntax_standards.hpp"
#include "lexer_factory.hpp"

#include <QSettings>
#include <QColor>
#include <QMap>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Dialog;

ThemeSettings::ThemeSettings(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::ThemeSettingsDialog())
{
	ui->setupUi(this);
	
	ui->table->setColumnWidth(0, 180);
	
	const int rows = ui->table->rowCount();
	m_boxes = new ColorBox *[rows];
	for(int i = 0; i < rows; ++i) {
		m_boxes[i] = new ColorBox(ui->table);
		m_boxes[i]->setProperty("row", i);
		connect(m_boxes[i], SIGNAL(colorChanged(QColor)), SLOT(settingChanged(QColor)));
		ui->table->setCellWidget(i, 1, m_boxes[i]);
	}
	readSettings();
	setDefaults();
	connect(ui->reset, SIGNAL(clicked()), SLOT(setDefaults()));
}

ThemeSettings::~ThemeSettings()
{
	for(quint16 i = 0; i < ui->table->rowCount(); ++i) delete m_boxes[i];
	delete m_boxes;
	delete ui;
}

int ThemeSettings::exec()
{
	readSettings();
	if(!QDialog::exec()) return QDialog::Rejected;
	writeSettings();
	return QDialog::Accepted;
}

const QMap<QString, QColor>& ThemeSettings::settings() const
{
	return m_lexerSettings;
}

QColor ThemeSettings::backgroundColor() const
{
	return ui->backgroundColorBox->color();
}

QFont ThemeSettings::font() const
{
	return ui->fontBox->currentFont();
}

int ThemeSettings::fontSize() const
{
	return ui->fontSizeBox->value();
}

void ThemeSettings::readSettings()
{
	QSettings settings;

	settings.beginGroup(EDITOR);
	ui->backgroundColorBox->setColor(settings.value(BACKGROUND_COLOR, QColor(255, 255, 255)).value<QColor>());

	// Figure out the font and set it
#ifdef Q_OS_WIN32
	QString fontString = settings.value(FONT, "Courier New").toString();
#elif defined(Q_OS_MAC)
	QString fontString = settings.value(FONT, "Monaco").toString();
#else
	QString fontString = settings.value(FONT, "Monospace").toString();
#endif
	
	ui->fontBox->setCurrentFont(QFont(fontString));
	
	int fontSize = 0;
#ifdef Q_OS_MAC
	fontSize = settings.value(FONT_SIZE, 12).toInt();
#else
	fontSize = settings.value(FONT_SIZE, 10).toInt();
#endif
	
	// Figure out the font size and set the widget
	ui->fontSizeBox->setValue(fontSize);
	
	// Read the lexer settings
	m_lexerSettings.clear();
	settings.beginGroup(LEXER);
	QStringList keys = settings.childKeys();
	foreach(const QString& key, keys) m_lexerSettings.insert(key, settings.value(key).value<QColor>());

	settings.endGroup();
	settings.endGroup();
	
	updateBoxes();
}

void ThemeSettings::writeSettings()
{
	QSettings settings;
	
	settings.beginGroup(EDITOR);
	
	settings.setValue(BACKGROUND_COLOR, backgroundColor());
	settings.setValue(FONT, font());
	settings.setValue(FONT_SIZE, fontSize());
	
	settings.beginGroup(LEXER);
	QMap<QString, QColor>::const_iterator i = m_lexerSettings.constBegin();
	for(; i != m_lexerSettings.constEnd(); ++i) settings.setValue(i.key(), i.value());
	Lexer::Settings::ref().setSettings(m_lexerSettings);
	settings.endGroup();
	settings.endGroup();
}

void ThemeSettings::setDefaults()
{
	m_lexerSettings[DEFAULT] = SyntaxStandards::defaultColor();
	m_lexerSettings[COMMENT] = SyntaxStandards::commentColor();
	m_lexerSettings[DOC_COMMENT] = SyntaxStandards::docColor();
	m_lexerSettings[NUMBER] = SyntaxStandards::numberColor();
	m_lexerSettings[KEYWORD] = SyntaxStandards::keywordColor();
	m_lexerSettings[STRING] = SyntaxStandards::stringColor();
	m_lexerSettings[PREPROCESSOR] = SyntaxStandards::preprocessorColor();
	m_lexerSettings[OPERATOR] = SyntaxStandards::operatorColor();
	m_lexerSettings[UNCLOSED_STRING] = SyntaxStandards::unclosedStringColor();
	m_lexerSettings[DOC_KEYWORD] = SyntaxStandards::docKeywordColor();
	m_lexerSettings[DOC_KEYWORD_ERROR] = SyntaxStandards::docKeywordErrorColor();
	updateBoxes();
}

void ThemeSettings::initializeDefaults()
{
	// This is sooo jank
	ThemeSettings *settings = new ThemeSettings(0);
	settings->writeSettings();
	delete settings;
}

void ThemeSettings::settingChanged(QColor color)
{
	ColorBox *box = (ColorBox *)sender();
	QString setting = ui->table->item(box->property("row").toInt(), 0)->text();
	m_lexerSettings.insert(setting, color);
}

void ThemeSettings::updateBoxes()
{
	m_boxes[0]->setColor(m_lexerSettings.value(DEFAULT, SyntaxStandards::defaultColor()));
	m_boxes[1]->setColor(m_lexerSettings.value(COMMENT, SyntaxStandards::commentColor()));
	m_boxes[2]->setColor(m_lexerSettings.value(DOC_COMMENT, SyntaxStandards::docColor()));
	m_boxes[3]->setColor(m_lexerSettings.value(NUMBER, SyntaxStandards::numberColor()));
	m_boxes[4]->setColor(m_lexerSettings.value(KEYWORD, SyntaxStandards::keywordColor()));
	m_boxes[5]->setColor(m_lexerSettings.value(STRING, SyntaxStandards::stringColor()));
	m_boxes[6]->setColor(m_lexerSettings.value(PREPROCESSOR, SyntaxStandards::preprocessorColor()));
	m_boxes[7]->setColor(m_lexerSettings.value(OPERATOR, SyntaxStandards::operatorColor()));
	m_boxes[8]->setColor(m_lexerSettings.value(UNCLOSED_STRING, SyntaxStandards::unclosedStringColor()));
	m_boxes[9]->setColor(m_lexerSettings.value(DOC_KEYWORD, SyntaxStandards::docKeywordColor()));
	m_boxes[10]->setColor(m_lexerSettings.value(DOC_KEYWORD_ERROR, SyntaxStandards::docKeywordErrorColor()));
}

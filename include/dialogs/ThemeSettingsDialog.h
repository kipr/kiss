#ifndef _THEMESETTINGSDIALOG_H_
#define _THEMESETTINGSDIALOG_H_

#include <QDialog>
#include <QMap>
#include <QAbstractButton>
#include "ColorBox.h"

#define LEXER "Lexer"
#define FONT "font"
#define FONT_SIZE "fontsize"
#define BACKGROUND_COLOR "backgroundcolor"

namespace Ui
{
	class ThemeSettingsDialog;
}

class ThemeSettingsDialog : public QDialog
{
Q_OBJECT
public:
	ThemeSettingsDialog(QWidget *parent = 0);
	~ThemeSettingsDialog();
	
	virtual int exec();
	
	const QMap<QString, QColor>& settings() const;
	QColor backgroundColor() const;
	QFont font() const;
	int fontSize() const;
	
	void readSettings();
	void writeSettings();
	
public slots:
	void setDefaults();
	
private slots:
	void settingChanged(QColor color);
	
private:
	void updateBoxes();
	
	Ui::ThemeSettingsDialog *ui;
	QMap<QString, QColor> m_lexerSettings;
	ColorBox **m_boxes;
	int m_fontSize;
	QFont m_font;
	QColor m_backgroundColor;
};

#endif

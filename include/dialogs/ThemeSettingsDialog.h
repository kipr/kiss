#ifndef THEMESETTINGSDIALOG_H
#define THEMESETTINGSDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "ColorBox.h"

namespace Ui
{
	class ThemeSettingsDialog;
}

class ThemeSettingsDialog : public QDialog
{
	Q_OBJECT
	
public:
	ThemeSettingsDialog(QMap<QString, QColor> lexerSettings, QColor backgroundColor, QFont font, int fontSize, QWidget *parent = 0);
	~ThemeSettingsDialog();
	
	QMap<QString, QColor> settings();
	QColor backgroundColor();
	QFont font();
	int fontSize();
	
private slots:
	void on_ui_buttonBox_clicked(QAbstractButton *button);
	void settingChanged(QColor color);
	
private:
	Ui::ThemeSettingsDialog *ui;
	QMap<QString, QColor> m_lexerSettings;
	ColorBox **boxes;
};

#endif // THEMESETTINGSDIALOG_H

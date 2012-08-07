#ifndef LEXERSETTINGSDIALOG_H
#define LEXERSETTINGSDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "ColorBox.h"

namespace Ui
{
	class LexerSettingsDialog;
}

class LexerSettingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    LexerSettingsDialog(QMap<QString, QColor> lexerSettings, QWidget *parent = 0);
    ~LexerSettingsDialog();
    
    QMap<QString, QColor> settings();
    
private slots:
	void on_ui_buttonBox_clicked(QAbstractButton *button);
	void settingChanged(QColor color);
    
private:
    Ui::LexerSettingsDialog *ui;
    QMap<QString, QColor> m_lexerSettings;
    ColorBox **boxes;
};

#endif // LEXERSETTINGSDIALOG_H

#ifndef LEXERSETTINGSDIALOG_H
#define LEXERSETTINGSDIALOG_H

#include <QDialog>
#include "ColorBox.h"

#include "ui_LexerSettingsDialog.h"

class LexerSettingsDialog : public QDialog, private Ui::LexerSettingsDialog
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

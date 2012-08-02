#ifndef LEXERSETTINGSDIALOG_H
#define LEXERSETTINGSDIALOG_H

#include <QDialog>

#include "ui_LexerSettingsDialog.h"

class LexerSettingsDialog : public QDialog, private Ui::LexerSettingsDialog
{
    Q_OBJECT
    
public:
    LexerSettingsDialog(QMap<QString, QColor> lexerSettings, QWidget *parent = 0);
    ~LexerSettingsDialog();
    
    QMap<QString, QColor> settings();
    
private slots:
	void settingChanged(QColor color);
    
private:
    Ui::LexerSettingsDialog *ui;
    QMap<QString, QColor> m_lexerSettings;
};

#endif // LEXERSETTINGSDIALOG_H

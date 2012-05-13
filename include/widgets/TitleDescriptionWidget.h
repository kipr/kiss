#ifndef _TITLEDESCRIPTIONWIDGET_H_
#define _TITLEDESCRIPTIONWIDGET_H_

#include "ui_TitleDescriptionWidget.h"
#include <QString>

class TitleDescriptionWidget : public QWidget, public Ui::TitleDescriptionWidget
{
Q_OBJECT
public:
	TitleDescriptionWidget(QWidget* parent);
	TitleDescriptionWidget(const QString& title, const QString& description, QWidget* parent);
	TitleDescriptionWidget(const QString& title, const QString& description, const QString& decoration, QWidget* parent);
};

#endif

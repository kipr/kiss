#include "TitleDescriptionWidget.h"

TitleDescriptionWidget::TitleDescriptionWidget(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
}

TitleDescriptionWidget::TitleDescriptionWidget(const QString& title, const QString& description, QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
	this->title->setText(title);
	this->description->setText(description);
}
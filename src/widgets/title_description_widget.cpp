#include "title_description_widget.hpp"

using namespace Kiss::Widget;

TitleDescription::TitleDescription(QWidget* parent)
	: QWidget(parent)
{
	setupUi(this);
}

TitleDescription::TitleDescription(const QString& title, const QString& description, QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	this->title->setText(title);
	this->description->setText(description);
}

TitleDescription::TitleDescription(const QString& title, const QString& description, const QString& decoration, QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	this->title->setText(title);
	this->description->setText(description);
	ui_decoration->setHtml(decoration);
	if(decoration.isEmpty()) ui_decoration->hide();
}
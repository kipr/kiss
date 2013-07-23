#include "icon_widget.hpp"

#include <QMouseEvent>

using namespace kiss;
using namespace kiss::widget;

Icon::Icon(QWidget *parent)
	: QLabel(parent)
{
	
}

void Icon::setIcon(const QIcon &icon)
{
	setPixmap(icon.pixmap(icon.availableSizes()[0]));
}

QIcon Icon::icon() const
{
	return QIcon(*pixmap());
}

void Icon::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}
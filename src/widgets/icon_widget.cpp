#include "icon_widget.hpp"

#include <QMouseEvent>

using namespace Kiss;
using namespace Kiss::Widget;

Icon::Icon(QWidget *parent)
	: QLabel(parent)
{
	
}

void Icon::setIcon(const QIcon& icon)
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
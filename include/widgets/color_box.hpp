#ifndef _COLORBOX_H_
#define _COLORBOX_H_

#include <QLineEdit>

class ColorBox : public QLineEdit
{
Q_OBJECT
public:
	ColorBox(QWidget *parent = 0);
	~ColorBox();
	QColor color() const;
	
signals:
	void colorChanged(QColor);
	
public slots:
	void setColor(QColor);

private:
	QColor m_color;
	
	void mouseDoubleClickEvent(QMouseEvent *);
};

#endif

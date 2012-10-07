#include "code_editor.hpp"

#include <QPaintEvent>
#include <QPainter>
#include <QTextBlock>

#include <QDebug>

using namespace Kiss;
using namespace Kiss::Widget;

CodeEditorMargin::CodeEditorMargin(CodeEditor *editor)
	: QWidget(editor),
	m_editor(editor)
{
}

QSize CodeEditorMargin::sizeHint() const
{
	return QSize(m_editor->marginSize(), 0);
}


void CodeEditorMargin::paintEvent(QPaintEvent *event)
{
	m_editor->marginPaintEvent(event);
}

MarginIndicator MarginIndicator::s_none(Qt::transparent);

MarginIndicator::MarginIndicator(const QColor& color)
	: m_color(color)
{
	
}

const QColor& MarginIndicator::color() const
{
	return m_color;
}

const MarginIndicator& MarginIndicator::none()
{
	return s_none;
}

CodeEditor::CodeEditor(QWidget *parent)
	: QPlainTextEdit(parent),
	m_margin(new CodeEditorMargin(this))
{
	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateMarginSize()));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateMarginArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
	connect(this, SIGNAL(cursorPositionChanged()), m_margin, SLOT(update()));

	updateMarginSize();
	highlightCurrentLine();
	
	setEditorFont(editorFont());
}

void CodeEditor::setEditorFont(const QFont& font)
{
	setFont(font);
	m_marginFont = font;
	m_marginFont.setPointSize(font.pointSize() - 2);
	setTabStopWidth(fontMetrics().width(QLatin1Char(' ')) * 4);
	updateMarginSize();
}

QFont CodeEditor::editorFont() const
{
	return font();
}

QFont CodeEditor::marginFont() const
{
	return m_marginFont;
}

void CodeEditor::setMarginIndicator(const int& line, const MarginIndicator& indicator)
{
	m_indicators.remove(line);
	m_indicators.insert(line, indicator);
	m_margin->update();
}

void CodeEditor::clearIndicators()
{
	m_indicators.clear();
}

quint16 CodeEditor::marginSize() const
{
	quint16 digits = 1;
	quint16 max = qMax(1, blockCount());
	while(max >= 10) {
		max /= 10;
		++digits;
	}
	quint16 space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
	return qMax(space, (quint16)(QFontMetrics(m_marginFont).width(QLatin1Char('9')) * 4));
}

void CodeEditor::marginPaintEvent(QPaintEvent *event)
{
	QPainter painter(m_margin);
	
	painter.fillRect(event->rect(), Qt::white);
	
	painter.setPen(QColor(127, 127, 127));
	
	
	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();

	painter.setFont(m_marginFont);

	quint16 height = QFontMetrics(painter.font()).height();

	while(block.isValid() && top <= event->rect().bottom()) {
		if(block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.drawText(0, top + 3, m_margin->width() - 4, height, Qt::AlignVCenter | Qt::AlignRight, number);
		}
		
		if(m_indicators.contains(blockNumber)) {
			painter.fillRect(0, top, m_margin->width() - 1, bottom - top, m_indicators.value(blockNumber, MarginIndicator::none()).color());
		}
		
		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		
		++blockNumber;
	}
	
	painter.drawLine(m_margin->width() - 1, event->rect().top(), m_margin->width() - 1, event->rect().bottom());
}

void CodeEditor::updateMarginArea(const QRect& rect, const int& dy)
{
	m_margin->update(0, rect.y(), (dy ? dy : m_margin->width()), rect.height());
	if(rect.contains(viewport()->rect())) updateMarginSize();
}

void CodeEditor::updateMarginSize()
{
	setViewportMargins(marginSize(), 0, 0, 0);
}

void CodeEditor::highlightCurrentLine()
{
	if(isReadOnly()) return;

	QTextEdit::ExtraSelection selection;
	
	QColor lineColor = QColor(QColor(240, 240, 240));
	
	selection.format.setBackground(lineColor);
	selection.format.setProperty(QTextFormat::FullWidthSelection, true);
	selection.cursor = textCursor();
	selection.cursor.clearSelection();
	
	setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);
	
	const QRect& cr = contentsRect();
	m_margin->setGeometry(QRect(cr.left(), cr.top(), marginSize(), cr.height()));
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
	QTextCursor c = textCursor();
	if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
		qDebug() << "Return pressed";
		int pos = c.positionInBlock();
		QString line = c.block().text();
		if(line.data()[pos - 1] == QChar('{')) {
			qDebug() << "Detected {";
			c.insertText("\n\t");
			ensureCursorVisible();
			return;
		}
		if(line.trimmed().isEmpty()) {
			c.insertText("\n");
			c.insertText(line);
			ensureCursorVisible();
			return;
		}
	}
	/* if(e->key() == '}') {
		QTextDocument *doc = document();
		QTextBlock block = Qdocument()->findBlock(c.blockNumber() - 1);
		QString line = block.text();
		int i = 0;
		for(; i < line.length() && line.data()[i] ; ++i);
		block.text().
		
	} */
	
	QPlainTextEdit::keyPressEvent(e);
	ensureCursorVisible();
}

void CodeEditor::scrollContentsBy(int dx, int dy)
{
	QPlainTextEdit::scrollContentsBy(dx, dy);
	if(!dy) return;
	m_margin->update();
}
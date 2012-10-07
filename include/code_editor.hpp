#ifndef _CODE_EDITOR_HPP_
#define _CODE_EDITOR_HPP_

#include <QMap>
#include <QWidget>
#include <QPlainTextEdit>

namespace Kiss
{
	namespace Widget
	{
		class CodeEditor;
		
		class CodeEditorMargin : public QWidget
		{
		public:
			CodeEditorMargin(CodeEditor *editor);
			QSize sizeHint() const;

		protected:
			void paintEvent(QPaintEvent *event);

		private:
			CodeEditor *m_editor;
		};
		
		class MarginIndicator
		{
		public:
			MarginIndicator(const QColor& color);
			const QColor& color() const;
			
			static const MarginIndicator& none();
		private:
			QColor m_color;
			
			static MarginIndicator s_none;
		};
		
		class CodeEditor : public QPlainTextEdit
		{
		Q_OBJECT
		public:
			CodeEditor(QWidget *parent = 0);
			
			void setEditorFont(const QFont& font);
			QFont editorFont() const;
			QFont marginFont() const;
			
			void setMarginIndicator(const int& line, const MarginIndicator& indicator);
			void clearIndicators();
			
			quint16 marginSize() const;
			
			friend class CodeEditorMargin;
		protected:
			void marginPaintEvent(QPaintEvent *event);
			void resizeEvent(QResizeEvent *e);
			void scrollContentsBy(int dx, int dy);
			
			void keyPressEvent(QKeyEvent *e);
			
		private slots:
			void highlightCurrentLine();
			void updateMarginArea(const QRect& rect, const int& dy);
			void updateMarginSize();
			
		private:
			QFont m_marginFont;
			CodeEditorMargin *m_margin;
			QMap<int, MarginIndicator> m_indicators;
		};
	}
}

#endif

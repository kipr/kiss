#include "output_widget.hpp"
#include "syntax_standards.hpp"

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QFileInfo>
#include <QDebug>

class OutputHighlighter : public QSyntaxHighlighter
{
public:
	struct Rule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	
	OutputHighlighter(QTextDocument *parent = 0)
		: QSyntaxHighlighter(parent)
	{
		OutputHighlighter::Rule rule;

		QTextCharFormat keywordFormat;
		keywordFormat.setForeground(SyntaxStandards::keywordColor());
		keywordFormat.setFontWeight(QFont::Bold);

		QStringList keywordPatterns;
		keywordPatterns << "auto" << "break" << "case" << "char"
			<< "const" << "continue" << "default" << "do"
			<< "double" << "else" << "enum" << "extern"
			<< "float" << "for" << "goto" << "if" << "int"
			<< "long" << "register" << "return" << "short"
			<< "signed" << "sizeof" << "static" << "struct"
			<< "switch" << "typedef" << "union" << "unsigned"
			<< "void" << "volatile" << "while" << "private"
			<< "public" << "protected";

		foreach(const QString& pattern, keywordPatterns) {
			rule.pattern = QRegExp(QString("\\b") + pattern + "\\b");
			rule.format = keywordFormat;
			addRule(rule);
		}
		
		QTextCharFormat errorFormat;
		errorFormat.setForeground(Qt::red);
		errorFormat.setFontWeight(QFont::Bold);
		rule.pattern = QRegExp("\\berror\\b");
		rule.format = errorFormat;
		addRule(rule);
		
		QTextCharFormat warningFormat;
		warningFormat.setForeground(QColor(255, 153, 51));
		warningFormat.setFontWeight(QFont::Bold);
		rule.pattern = QRegExp("\\bwarning\\b");
		rule.format = warningFormat;
		addRule(rule);
		
		QTextCharFormat noteFormat;
		noteFormat.setForeground(Qt::darkGray);
		noteFormat.setFontWeight(QFont::Bold);
		rule.pattern = QRegExp("\\bnote\\b");
		rule.format = noteFormat;
		addRule(rule);
		
		QTextCharFormat numberFormat;
		numberFormat.setFontWeight(QFont::Bold);

		rule.pattern = QRegExp("\\b\\d+((l|L)|(e\\d))?\\b");
		rule.format = numberFormat;
		addRule(rule);

		rule.pattern = QRegExp("\\b0x[0-9A-Fa-f]+\\b");
		rule.format = numberFormat;
		addRule(rule);
	}
	
	void highlightBlock(const QString& text)
	{
		foreach(const Rule& rule, m_highlightingRules) {
			QRegExp expression(rule.pattern);
			int index = expression.indexIn(text);
			while(index >= 0) {
				int length = expression.matchedLength();
				setFormat(index, length, rule.format);
				index = expression.indexIn(text, index + length);
			}
		}
		
		setCurrentBlockState(0);
	}
private:
	void addRule(const OutputHighlighter::Rule& rule)
	{
		m_highlightingRules.append(rule);
	}
	
	QVector<Rule> m_highlightingRules;
};

OutputWidget::OutputWidget(QWidget *parent)
	: QTextBrowser(parent)
{
	new OutputHighlighter(document());
	QFont font("Monospace");
	font.setStyleHint(QFont::TypeWriter);
	font.setPointSize(14);
	setFont(font);
	setReadOnly(true);
}

void OutputWidget::setOutputList(const Compiler::OutputList& outputList)
{
	m_outputList = outputList;
	QString out;
	foreach(const Compiler::Output& output, outputList) {
		QString errorString = processString(output.error().trimmed());
		QString outputString = processString(output.output().trimmed());
		
		if(errorString.isEmpty() && outputString.isEmpty()) continue;
		
		out += "<p>";
		QStringList transformedFiles;
		foreach(const QString& file, output.files()) {
			transformedFiles << QFileInfo(file).fileName();
		}
		if(outputList.size() > 1 && !transformedFiles.isEmpty()) {
			out += "<b>" + transformedFiles.join(", ") + ":</b> <br />";
		}
		if(!errorString.isEmpty()) out += "<pre>" + errorString + "\n</pre>";
		if(!outputString.isEmpty()) out += "<pre>" + outputString + "</pre>\n";
		out += "</p>";
	}
	out = out.trimmed();
	setVisible(!out.isEmpty());
	setHtml(out);
}

const Compiler::OutputList& OutputWidget::outputList() const
{
	return m_outputList;
}

QString OutputWidget::processString(const QString& string)
{
	QString ret = string;
	ret.replace("<", "&lt;");
	ret.replace(">", "&gt;");
	QRegExp rx("([a-zA-Z]:)?(/[a-zA-Z0-9_.-]+)*([a-zA-Z0-9_-]+\\.[a-zA-Z]+)/?:");
	int pos = 0;
	while ((pos = rx.indexIn(ret, pos)) != -1) {
		const QString path = ret.mid(pos, rx.matchedLength() - 1);
		const QString html = "<a href=\"kiss://" + path + "\">" + QFileInfo(path).fileName() + "</a>";
		ret.replace(pos, path.length(), html);
		pos += html.length();
	}
	ret.replace("\n", "<br />");
	return ret;
}

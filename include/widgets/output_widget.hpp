#ifndef _OUTPUT_WIDGET_HPP_
#define _OUTPUT_WIDGET_HPP_

#include <QTextBrowser>
#include <QString>
#include <pcompiler/output.hpp>

class OutputWidget : public QTextBrowser
{
Q_OBJECT
public:
	OutputWidget(QWidget *parent = 0);
	
	void setOutputList(const Compiler::OutputList& outputList);
	const Compiler::OutputList& outputList() const;
	
private:
	QString processString(const QString& string);
	
	Compiler::OutputList m_outputList;
};

#endif

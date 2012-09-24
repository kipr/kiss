#ifndef _OUTPUTWIDGET_H_
#define _OUTPUTWIDGET_H_

#include <QTextBrowser>
#include <pcompiler/output.hpp>

class OutputWidget : public QTextBrowser
{
Q_OBJECT
public:
	OutputWidget(QWidget *parent = 0);
	
	void setOutputList(const Compiler::OutputList& outputList);
	const Compiler::OutputList& outputList() const;
	
private:
	Compiler::OutputList m_outputList;
};

#endif

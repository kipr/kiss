#include "output_widget.hpp"

OutputWidget::OutputWidget(QWidget *parent)
	: QTextBrowser(parent)
{
	
}

void OutputWidget::setOutputList(const Compiler::OutputList& outputList)
{
	m_outputList = outputList;
}

const Compiler::OutputList& OutputWidget::outputList() const
{
	return m_outputList;
}
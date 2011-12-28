#include "SourceFindWidget.h"

#include "SourceFile.h"

SourceFindWidget::SourceFindWidget(QWidget* parent) : QWidget(parent), m_sourceFile(0)
{
	setupUi(this);
}

void SourceFindWidget::setSourceFile(SourceFile* sourceFile)
{
	m_sourceFile = sourceFile;
}

void SourceFindWidget::setModified(bool m)
{
	m_findModified = m;
}

bool SourceFindWidget::isModified()
{
	return m_findModified;
}

void SourceFindWidget::show()
{
	ui_find->clear();
	ui_replace->clear();
	QWidget::show();
}

void SourceFindWidget::on_ui_next_clicked()
{
	if(m_findModified) m_sourceFile->editor()->findFirst(ui_find->text(), false, ui_matchCase->isChecked(), false, true);
	else m_sourceFile->editor()->findNext();
	m_findModified = false;
}

void SourceFindWidget::on_ui_find_textChanged(const QString&)
{
	m_findModified = true;
}

void SourceFindWidget::on_ui_matchCase_stateChanged(int)
{
	m_findModified = true;
}

void SourceFindWidget::on_ui_replaceNext_clicked()
{
	m_sourceFile->editor()->replace(ui_replace->text()); on_ui_next_clicked();
}

void SourceFindWidget::on_ui_replaceAll_clicked()
{
	m_sourceFile->editor()->setText(m_sourceFile->editor()->text().replace(ui_find->text(), ui_replace->text()));
}
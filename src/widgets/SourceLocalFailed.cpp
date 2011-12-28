#include "SourceLocalFailed.h"

#include "MainWindow.h"
#include "SourceFile.h"

SourceLocalFailed::SourceLocalFailed(QWidget* parent) : QWidget(parent), m_sourceFile(0), m_state(Ask), m_filePath("")
{
	setupUi(this);
}

void SourceLocalFailed::setSourceFile(SourceFile* sourceFile)
{
	m_sourceFile = sourceFile;
}

void SourceLocalFailed::performAction(const QString filePath)
{
	m_filePath = filePath;
	
	if(m_state == Always && m_sourceFile->target()->hasRawDownload())
		m_sourceFile->target()->rawDownload(m_filePath);
	else show();
}

void SourceLocalFailed::setState(int state)
{
	m_state = state;
}

int SourceLocalFailed::state()
{
	return m_state;
}

void SourceLocalFailed::on_ui_always_clicked()
{
	on_ui_yes_clicked();
	m_state = Always;
}

void SourceLocalFailed::on_ui_yes_clicked()
{
	hide();
	MainWindow::ref().setStatusMessage("Downloading Anyway...");
	QApplication::flush();
	MainWindow::ref().setStatusMessage(m_sourceFile->target()->rawDownload(m_filePath) ? tr("Download Succeeded") : tr("Download Failed"));
}

void SourceLocalFailed::on_ui_no_clicked() { hide(); }

void SourceLocalFailed::on_ui_never_clicked()
{
	on_ui_no_clicked();
	m_state = Never;
}
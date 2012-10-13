#include "save_as_dialog.hpp"

#include "ui_save_as_dialog.h"

#include <QFileSystemModel>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QPushButton>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Dialog;

SaveAs::SaveAs(QWidget *parent)
	: QDialog(parent),
	m_model(new QFileSystemModel(this)),
	ui(new Ui::SaveAsDialog)
{
	ui->setupUi(this);
	
	ui->tree->setModel(m_model);
	ui->tree->header()->setResizeMode(0, QHeaderView::Stretch);
	
	connect(ui->tree->selectionModel(),
		SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
		SLOT(selectionChanged(QModelIndex)));
	connect(ui->file,
		SIGNAL(textChanged(QString)),
		SLOT(fileNameChanged(QString)));
}

SaveAs::~SaveAs()
{
	delete m_model;
	delete ui;
}

void SaveAs::setNameFilters(const QStringList& filters)
{
	m_model->setNameFilters(filters);
}

QStringList SaveAs::nameFilters() const
{
	return m_model->nameFilters();
}

void SaveAs::setFileName(const QString& name)
{
	ui->file->setText(name);
}

QString SaveAs::fileName() const
{
	return ui->file->text();
}

void SaveAs::setRootPath(const QString& path)
{
	ui->tree->setRootIndex(m_model->setRootPath(path));
}

QString SaveAs::rootPath() const
{
	return m_model->rootPath();
}
	
QString SaveAs::filePath() const
{
	QModelIndexList indexes = ui->tree->selectionModel()->selectedRows();
	if(indexes.size() != 1) return QDir(rootPath()).filePath(fileName());
	
	QModelIndex index = indexes[0];
	QString path = m_model->isDir(index) ? m_model->filePath(index)
		: QFileInfo(m_model->filePath(index)).path();
	
	return QDir(path).filePath(fileName());
}

void SaveAs::fileNameChanged(const QString& name)
{
	QStringList nf = nameFilters();
	bool match = nf.isEmpty();
	foreach(const QString& nameFilter, nf) {
		QRegExp expr(nameFilter, Qt::CaseInsensitive, QRegExp::Wildcard);
		if(expr.exactMatch(name)) {
			match = true;
			break;
		}
	}
	
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(match && !name.isEmpty());
}

void SaveAs::selectionChanged(const QModelIndex& index)
{
	if(m_model->isDir(index)) return;
	
	setFileName(m_model->fileName(index));
}
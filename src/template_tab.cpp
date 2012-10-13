#include "template_tab.hpp"

#include "ui_template_tab.h"

#include "template_pack.hpp"
#include "template_pack_menu.hpp"
#include "template_model.hpp"
#include "template_manager.hpp"
#include "main_window.hpp"

#include <QFileDialog>
#include <QMessageBox>

using namespace Kiss;
using namespace Kiss::Widget;

TemplateTab::TemplateTab(const Template::PackPtr& pack, MainWindow *mainWindow)
	: QWidget(mainWindow),
	Tab(this, mainWindow),
	m_dirty(true),
	m_manager(new Template::Manager()),
	m_pack(pack),
	m_model(0),
	ui(new Ui::TemplateTab())
{
	init();
}

TemplateTab::TemplateTab(const QString& path, MainWindow *mainWindow)
	: QWidget(mainWindow),
	Tab(this, mainWindow),
	m_dirty(false),
	m_manager(new Template::Manager()),
	m_pack(Template::Pack::load(path)),
	m_model(0),
	ui(new Ui::TemplateTab())
{
	setFile(path);
	init();
}

TemplateTab::~TemplateTab()
{
	selectionChanged(QItemSelection(), ui->packTree->selectionModel()->selection());
	
	delete ui;
	delete m_manager;
}

void TemplateTab::activate()
{
	mainWindow()->deactivateMenuablesExcept(mainWindow()->standardMenus());
	mainWindow()->activateMenuable(Menu::TemplatePackMenu::menuName(), this);
}

bool TemplateTab::beginSetup()
{
	return true;
}

void TemplateTab::completeSetup()
{
	updateTitle();
}

bool TemplateTab::close()
{
	if(!m_dirty) return true;
	
	QMessageBox::StandardButton ret = QMessageBox::question(this, tr("Unsaved Changes"),
		tr("Save Changes to \"") + file().fileName() + tr("\" before closing?"),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	if(ret == QMessageBox::Cancel) return false;
	if(ret == QMessageBox::Yes) {
		save();
		if(m_dirty) return false;
	}
	
	return true;
}

void TemplateTab::refreshSettings()
{
	// TODO: Does anything need to go here?
}


bool TemplateTab::saveAs(const QString& file)
{
	setFile(file);
	m_pack->save(Tab::file().absoluteFilePath());
	m_dirty = false;
	updateTitle();
	return true;
}

bool TemplateTab::addFile(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return false;
	QFileInfo info(path);
	Template::File tFile(file.readAll(), info.completeSuffix());
	bool ret = m_pack->addFile(info.baseName(), tFile);
	file.close();
	dirty();
	return ret;
}

bool TemplateTab::save()
{
	return hasFile() ? saveAs(file().absoluteFilePath()) : saveAs();
}

bool TemplateTab::saveAs()
{
	QString path = QFileDialog::getSaveFileName(this,
		tr("Save Template Pack"), QString(), "Template Pack (*.pack)");
	if(path.isEmpty()) return false;
	saveAs(path);
	return true;
}

void TemplateTab::test()
{
	
}

void TemplateTab::addFile()
{
	QStringList files = QFileDialog::getOpenFileNames(this,
		tr("Which files would you like to add to the template pack?"));
	
	foreach(const QString& file, files) {
		addFile(file);
	}
	
	ui->packTree->expandAll();
}

void TemplateTab::deleteSelection()
{
	QModelIndexList selection = ui->packTree->selectionModel()->selectedRows();
	foreach(const QModelIndex& index, selection) {
		m_pack->removeFile(m_model->itemFromIndex(index)->text());
	}
	
	dirty();
}

void TemplateTab::selectionChanged(const QItemSelection& selection, const QItemSelection& prev)
{
	if(prev.indexes().size() == 1) {
		const QModelIndex index = prev.indexes()[0];
		QString text = m_model->itemFromIndex(index)->text();
		QString path = (m_model->isIndexPack(index) ? QString() : text);
		m_pack->setDescription(path, ui->description->toPlainText());
	}
	
	bool allFiles = true;
	foreach(const QModelIndex& index, selection.indexes()) {
		allFiles &= m_model->isIndexFile(index);
	}
	
	ui->deleteSelection->setEnabled(allFiles);
	
	const bool singular = selection.indexes().size() == 1;
	ui->file->setEnabled(singular);
	ui->description->setEnabled(singular);
	ui->descriptionLabel->setEnabled(singular);
	
	if(!singular) return;
	
	const QModelIndex index = selection.indexes()[0];
	
	QString text = m_model->itemFromIndex(index)->text();
	QString path = (m_model->isIndexPack(index) ? QString() : text);
	
	ui->file->setText(text + ":");
	ui->description->setText(m_pack->description(path).trimmed());
}

void TemplateTab::dirtyDesc()
{
	if(ui->description->document()->availableUndoSteps()
		|| ui->description->document()->availableRedoSteps()) dirty();
}

void TemplateTab::dirty()
{
	m_dirty = true;
	updateTitle();
}

void TemplateTab::updateTitle()
{
	mainWindow()->setTabName(this, (m_dirty ? "* " : "")
		+ (hasFile() ? file().fileName() : tr("Untitled")));
}

void TemplateTab::init()
{
	ui->setupUi(this);
	m_manager->addPack(m_pack);
	m_model = new Kiss::Template::Model(m_manager, ui->packTree);
	
	m_model->setReadOnly(false);
	ui->packTree->setModel(m_model);
	
	connect(ui->addFile, SIGNAL(clicked()), this, SLOT(addFile()));
	connect(ui->deleteSelection, SIGNAL(clicked()), this, SLOT(deleteSelection()));
	connect(ui->packTree->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
		this, SLOT(selectionChanged(QItemSelection, QItemSelection)));
	connect(m_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(dirty()));
	connect(ui->description, SIGNAL(textChanged()), this, SLOT(dirtyDesc()));
		
	selectionChanged(QItemSelection(), QItemSelection());
	
	ui->packTree->expandAll();
}
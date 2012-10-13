#include "project_settings.hpp"

#include "main_window.hpp"
#include "activatable.hpp"
#include "project.hpp"

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QDebug>

#define CUSTOM_KEY "<custom>"

using namespace Kiss;
using namespace Kiss::Widget;

class ComboDelegate : public QStyledItemDelegate
{
public:
	ComboDelegate(QObject *parent, const QStringList& keys)
		: QStyledItemDelegate(parent)
	{
		this->keys = keys;
	}

	virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
	{
		if(index.column() != 0) return QStyledItemDelegate::createEditor(parent, option, index);

		QComboBox *box = new QComboBox(parent);
		box->addItems(keys);
		box->addItem(CUSTOM_KEY);

		return box;
	}
	
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QComboBox *box = qobject_cast<QComboBox*>(editor);
		if(box) {
			QString currentText = index.data(Qt::EditRole).toString();
			int boxIndex = box->findText(currentText);
			if(boxIndex >= 0) box->setCurrentIndex(boxIndex);
		} else QStyledItemDelegate::setEditorData(editor, index);
	}
	
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *box = qobject_cast<QComboBox *>(editor);
		
		if(box) model->setData(index, box->currentText(), Qt::EditRole);
		else QStyledItemDelegate::setModelData(editor, model, index);
	}

private:
	QStringList keys;
};

ProjectSettings::ProjectSettings(const Project::ProjectPtr& project, MainWindow *mainWindow)
	: QWidget(mainWindow),
	Tab(this, mainWindow),
	m_block(false)
{
	setupUi(this);

	// ui_settingsTable->setItemDelegate(new ComboDelegate(ui_settingsTable, CompilerManager::ref().keys()));

	setProject(project);
	reload();
}

void ProjectSettings::activate()
{
	QList<Menu::Menuable*> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus());
	foreach(Menu::Menuable* menu, menus) {
		ActivatableObject* activatable = dynamic_cast<ActivatableObject *>(menu);
		if(activatable) activatable->setActive(0);
	}

	mainWindow()->showProjectDock();
}

bool ProjectSettings::beginSetup()
{
	return true;
}

void ProjectSettings::completeSetup()
{
	mainWindow()->setTabName(this, project()->name() + tr(" Settings"));
	// connect(project(), SIGNAL(settingsChanged()), SLOT(reload()));
}

bool ProjectSettings::close()
{
	// project()->save();
	return true;
}

void ProjectSettings::refreshSettings()
{

}

void ProjectSettings::reload()
{
	const Project::Project::Settings& settings = project()->settings();

	m_block = true;

	ui_settingsTable->clearContents();
	ui_settingsTable->setRowCount(settings.keys().size());
	
	quint16 i = 0;
	foreach(const QString& key, settings.keys()) {
		ui_settingsTable->setItem(i, 0, new QTableWidgetItem(key));
		ui_settingsTable->setItem(i, 1, new QTableWidgetItem(settings[key]));

		++i;
	}

	m_block = false;
}

void ProjectSettings::on_ui_settingsTable_cellChanged(int row, int column)
{
	if(m_block) return;

	if(column == 0 && ui_settingsTable->item(row, column)->text() == CUSTOM_KEY) {
		bool accepted = false;
		QString text = QInputDialog::getText(this, tr("Custom Key"), tr("Enter key:"), QLineEdit::Normal, "", &accepted);
		ui_settingsTable->setItem(row, column, new QTableWidgetItem(accepted ? text : ""));
	}

	QTableWidgetItem *key = ui_settingsTable->item(row, 0);
	QTableWidgetItem *value = ui_settingsTable->item(row, 1);

	if(key && value) project()->setSetting(key->text(), value->text());
}

void ProjectSettings::on_ui_add_clicked()
{
	ui_settingsTable->insertRow(ui_settingsTable->rowCount());
}

void ProjectSettings::on_ui_remove_clicked()
{
	int row = ui_settingsTable->currentRow();
	QTableWidgetItem* key = ui_settingsTable->item(row, 0);
	QString keyString = key ? key->text() : "";
	ui_settingsTable->removeRow(row);

	project()->removeSetting(keyString);
}

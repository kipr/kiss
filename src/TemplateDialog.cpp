#include "TemplateDialog.h"
#include "TargetManager.h"
#include <QListWidgetItem>
#include <QDebug>

TemplateDialog::TemplateDialog(QWidget* parent) : QDialog(parent)
{
	setupUi(this);
}

int TemplateDialog::exec()
{
	ui_targets->clear();
	
	const QStringList& targets = TargetManager::ref().targets();
	bool first = true;
	foreach(const QString& target, targets) {
		QListWidgetItem* item = new QListWidgetItem(TargetManager::ref().displayName(target));
		
		item->setData(Qt::UserRole, target);
		ui_targets->addItem(item);
		if(first) {
			ui_targets->setCurrentItem(item);
			first = false;
		}
	}
	
	return QDialog::exec();
}

int TemplateDialog::execTarget()
{
	ui_templates->hide();
	setFixedSize(200, 200);
	setWindowTitle("Targets");
	return exec();
}

QString TemplateDialog::selectedTargetFilePath()
{
	const QString& target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	return TargetManager::ref().targetPath(target) + "/" + target + ".target";
}

QString TemplateDialog::templateFile()
{
	const QString& target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	const QString& _template = ui_templates->currentItem() ? ui_templates->currentItem()->data(0, Qt::UserRole).toString() : "Default";
	return TargetManager::ref().targetPath(target) + "/templates/" + _template;
}

void TemplateDialog::on_ui_targets_currentItemChanged(QListWidgetItem* current, QListWidgetItem* prev)
{
	Q_UNUSED(prev);
	ui_templates->clear();
	const QString& target = current->data(Qt::UserRole).toString();
	const QStringList& templates = TargetManager::ref().templates(target);
	const QStringList& folders = TargetManager::ref().templateFolders(target);
	bool first = true;
	foreach(const QString& _template, templates) {
		const QString& templateName = _template.section(".", 0, 0);
		QTreeWidgetItem* item = new QTreeWidgetItem(ui_templates);
		item->setText(0, templateName);
		item->setIcon(0, TargetManager::ref().templateIcon(target, templateName));
		item->setData(0, Qt::UserRole, _template);
		
		ui_templates->addTopLevelItem(item);
		
		if(first) {
			ui_templates->setCurrentItem(item);
			first = false;
		}
	}
	foreach(const QString& folder, folders) {
		QTreeWidgetItem* item = new QTreeWidgetItem(ui_templates);
		item->setText(0, folder);
		item->setIcon(0, QIcon(":/welcome/document-open.png"));
		item->setFlags(Qt::NoItemFlags);
		const QStringList& templates = TargetManager::ref().templates(target, folder);
		foreach(const QString& _template, templates) {
			const QString& templateName = _template.section(".", 0, 0);
			QTreeWidgetItem* cItem = new QTreeWidgetItem(item);
			cItem->setText(0, templateName);
			cItem->setIcon(0, TargetManager::ref().templateIcon(target, templateName, folder));
			cItem->setData(0, Qt::UserRole, folder + "/" + _template);

			item->addChild(cItem);
		}
	}
}
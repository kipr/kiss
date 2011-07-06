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
	QString target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	return TargetManager::ref().targetPath(target) + "/" + target + ".target";
}

QString TemplateDialog::templateFile()
{
	QString target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	return TargetManager::ref().targetPath(target) + "/templates/" + ui_templates->currentItem()->text() + ".template";
}

void TemplateDialog::on_ui_targets_currentItemChanged(QListWidgetItem* current, QListWidgetItem* prev)
{
	ui_templates->clear();
	const QString& target = current->data(Qt::UserRole).toString();
	const QStringList& templates = TargetManager::ref().templates(target);
	bool first = true;
	foreach(const QString& _template, templates) {
		const QString& templateName = _template.section(".", 0, 0);
		QListWidgetItem* item = new QListWidgetItem(
			TargetManager::ref().templateIcon(target, templateName), 
			templateName);
		
		ui_templates->addItem(item);
		
		if(first) {
			ui_templates->setCurrentItem(item);
			first = false;
		}
	}
}
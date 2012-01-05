/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "TemplateDialog.h"
#include "TargetManager.h"
#include "TemplateManager.h"
#include "MessageDialog.h"
#include "Kiss.h"
#include "UiEventManager.h"

#include <QListWidgetItem>
#include <QDebug>

TemplateDialog::TemplateDialog(QWidget* parent) : QDialog(parent) { setupUi(this); }

int TemplateDialog::exec()
{
	ui_targets->clear();
	
	const QStringList& targets = TargetManager::ref().targets();
	if(targets.size() == 0) {
		MessageDialog::showError(this, "simple_error", QStringList() << 
			tr("You have no targets installed") <<
			tr("There is probably an issue with your installation. "
			"If you are a developer, you've forgotten to install the initial targets."));
		return QDialog::Rejected;
	}
	
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
	ui_remove->hide();
	setFixedSize(200, 200);
	setWindowTitle(tr("Targets"));
	return exec();
}

QString TemplateDialog::selectedTargetFilePath()
{
	const QString& target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	return TargetManager::ref().targetPath(target) + "/" + target + "." + TARGET_EXT;
}

QString TemplateDialog::templateFile()
{
	const QString& target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	return ui_templates->currentItem() ? 
		ui_templates->currentItem()->data(0, Qt::UserRole).toString() : 
		TemplateManager::ref().pathForTemplate(target, "Default");
}

void TemplateDialog::on_ui_targets_currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
	const QString& target = current->data(Qt::UserRole).toString();
	ui_templates->clear();
	addTemplates(target, 0, "");
	addUserTemplates(target);
	ui_templates->setCurrentItem(ui_templates->topLevelItem(0));
}

void TemplateDialog::on_ui_templates_itemDoubleClicked(QTreeWidgetItem* current)
{
	if(current->childCount() != 0) return;
	accept();
}

void TemplateDialog::on_ui_templates_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem*)
{
	if(!current) {
		ui_remove->setEnabled(false);
		return;
	}
	
	const QString& templatePath = current->data(0, Qt::UserRole).toString();
	const QString& target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	const QString& userTemplateRoot = TemplateManager::ref().pathForUserTemplate(target, "");
	ui_remove->setEnabled(templatePath.startsWith(userTemplateRoot));
}

void TemplateDialog::on_ui_remove_clicked()
{
	const QString& templatePath = ui_templates->currentItem()->data(0, Qt::UserRole).toString();
	const QString& target = ui_targets->currentItem()->data(Qt::UserRole).toString();
	TemplateManager::ref().deleteUserTemplate(target, QFileInfo(templatePath).fileName());
	
	on_ui_targets_currentItemChanged(ui_targets->currentItem(), 0);
}

void TemplateDialog::addTemplates(const QString& target, QTreeWidgetItem* parentItem, const QString& parent = "")
{
	const QStringList& templates = TemplateManager::ref().templates(target, parent);
	foreach(const QString& _template, templates) {
		const QString& templateName = _template.section(".", 0, 0).section("/", -1);
		QTreeWidgetItem* item = !parentItem ? new QTreeWidgetItem(ui_templates) : new QTreeWidgetItem(parentItem);
		item->setText(0, templateName);
		item->setIcon(0, TemplateManager::ref().templateIcon(target, _template));
		item->setData(0, Qt::UserRole, TemplateManager::ref().pathForTemplate(target, _template));
	}
	
	const QStringList& folders = TemplateManager::ref().templateFolders(target, parent);
	foreach(const QString& folder, folders) {
		QTreeWidgetItem* item = !parentItem ? new QTreeWidgetItem(ui_templates) : new QTreeWidgetItem(parentItem);
		item->setText(0, folder);
		item->setIcon(0, QIcon(":/welcome/document-open.png"));
		item->setFlags(Qt::ItemIsEnabled);
		
		addTemplates(target, item, (parent.isEmpty() ? "" : parent + "/") + folder);
	}
}

void TemplateDialog::addUserTemplates(const QString& target)
{
	const QStringList& templates = TemplateManager::ref().userTemplates(target);
	if(templates.size() == 0) return;
	
	QTreeWidgetItem* parent = new QTreeWidgetItem(ui_templates);
	parent->setText(0, tr("User Templates"));
	parent->setIcon(0, QIcon(":/welcome/document-open.png"));
	parent->setFlags(Qt::ItemIsEnabled);
	
	foreach(const QString& _template, templates) {
		const QString& templateName = _template.section(".", 0, 0).section("/", -1);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		item->setText(0, templateName);
		item->setIcon(0, QIcon(":/icon_set/icons/empty.png"));
		item->setData(0, Qt::UserRole, TemplateManager::ref().pathForUserTemplate(target, _template));
	}
}
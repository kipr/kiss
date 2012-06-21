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
#include "TemplateManager.h"
#include "MessageDialog.h"
#include "Kiss.h"
#include "UiEventManager.h"

#include "ResourceHelper.h"

#include <QListWidgetItem>
#include <QDebug>
#include <QFileInfo>

TemplateDialog::TemplateDialog(QWidget* parent) : QDialog(parent) { setupUi(this); }

int TemplateDialog::exec()
{
	ui_types->clear();
	
	const QStringList& types = TemplateManager::ref().types();
	bool first = true;
	foreach(const QString& type, types) {
		QListWidgetItem* item = new QListWidgetItem(type);
		
		item->setData(Qt::UserRole, type);
		ui_types->addItem(item);
		if(first) {
			ui_types->setCurrentItem(item);
			first = false;
		}
	}
	
	return QDialog::exec();
}

QString TemplateDialog::selectedTypeName() const
{
	return ui_types->currentItem() ? ui_types->currentItem()->data(Qt::UserRole).toString() : QString();
}

QString TemplateDialog::templateFile()
{
	if(!ui_types->currentItem()) return QString();
	const QString& type = ui_types->currentItem()->data(Qt::UserRole).toString();
	return ui_templates->currentItem() ? 
		ui_templates->currentItem()->data(0, Qt::UserRole).toString() : 
		TemplateManager::ref().pathForTemplate(type, "Default");
}

void TemplateDialog::on_ui_types_currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
	const QString& type = current->data(Qt::UserRole).toString();
	ui_templates->clear();
	addTemplates(type, 0, "");
	addUserTemplates(type);
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
	const QString& type = ui_types->currentItem()->data(Qt::UserRole).toString();
	const QString& userTemplateRoot = TemplateManager::ref().pathForUserTemplate(type, "");
	ui_remove->setEnabled(templatePath.startsWith(userTemplateRoot));
}

void TemplateDialog::on_ui_remove_clicked()
{
	const QString& templatePath = ui_templates->currentItem()->data(0, Qt::UserRole).toString();
	const QString& type = ui_types->currentItem()->data(Qt::UserRole).toString();
	TemplateManager::ref().deleteUserTemplate(type, QFileInfo(templatePath).fileName());
	
	on_ui_types_currentItemChanged(ui_types->currentItem(), 0);
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
		item->setIcon(0, ResourceHelper::ref().icon("open"));
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
	parent->setIcon(0, ResourceHelper::ref().icon("open"));
	parent->setFlags(Qt::ItemIsEnabled);
	
	foreach(const QString& _template, templates) {
		const QString& templateName = _template.section(".", 0, 0).section("/", -1);
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		item->setText(0, templateName);
		item->setIcon(0, QIcon(":/icon_set/icons/empty.png"));
		item->setData(0, Qt::UserRole, TemplateManager::ref().pathForUserTemplate(target, _template));
	}
}
/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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
#include "Kiss.h"
#include <QListWidgetItem>
#include <QDebug>

TemplateDialog::TemplateDialog(QWidget* parent) : QDialog(parent) { setupUi(this); }

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
	const QString& _template = ui_templates->currentItem() ? ui_templates->currentItem()->data(0, Qt::UserRole).toString() : "Default";
	return TargetManager::ref().targetPath(target) + "/templates/" + _template;
}

void TemplateDialog::on_ui_targets_currentItemChanged(QListWidgetItem* current, QListWidgetItem*)
{
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
		item->setFlags(Qt::ItemIsEnabled);
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

void TemplateDialog::on_ui_templates_itemDoubleClicked(QTreeWidgetItem* current)
{
	if(current->childCount() == 0) accept();
}

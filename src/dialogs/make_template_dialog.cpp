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

#include "make_template_dialog.hpp"

MakeTemplateDialog::MakeTemplateDialog(QWidget* parent) : QDialog(parent) { setupUi(this); }

void MakeTemplateDialog::setName(const QString& name) { ui_name->setText(name); }
void MakeTemplateDialog::setTypes(const QStringList& types) { ui_types->addItems(types); }
void MakeTemplateDialog::setExtension(const QString& extension) { ui_extension->setText(extension); }

QString MakeTemplateDialog::name() { return ui_name->text(); }
QString MakeTemplateDialog::type() { return ui_types->currentText(); }
QString MakeTemplateDialog::extension() { return ui_extension->text(); }
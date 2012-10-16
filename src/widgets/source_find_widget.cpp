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

#include "source_find_widget.hpp"

#include "source_file.hpp"

using namespace Kiss;
using namespace Kiss::Widget;

SourceFind::SourceFind(QWidget* parent)
	: QWidget(parent),
	m_sourceFile(0)
{
	setupUi(this);
}

void SourceFind::setSourceFile(SourceFile* sourceFile)
{
	m_sourceFile = sourceFile;
}

void SourceFind::setModified(const bool& m)
{
	m_findModified = m;
}

bool SourceFind::isModified() const
{
	return m_findModified;
}

void SourceFind::show()
{
	ui_find->clear();
	ui_replace->clear();
	QWidget::show();
	ui_find->setFocus(Qt::PopupFocusReason);
}

void SourceFind::on_ui_next_clicked()
{
	if(m_findModified) m_sourceFile->editor()->findFirst(ui_find->text(), false, ui_matchCase->isChecked(), false, true);
	else m_sourceFile->editor()->findNext();
	m_findModified = false;
}

void SourceFind::on_ui_find_textChanged(const QString&)
{
	m_findModified = true;
}

void SourceFind::on_ui_matchCase_stateChanged(int)
{
	m_findModified = true;
}

void SourceFind::on_ui_replaceNext_clicked()
{
	m_sourceFile->editor()->replace(ui_replace->text()); on_ui_next_clicked();
}

void SourceFind::on_ui_replaceAll_clicked()
{
	m_sourceFile->editor()->setText(m_sourceFile->editor()->text().replace(ui_find->text(), ui_replace->text()));
}
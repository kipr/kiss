#include "info_box.hpp"

using namespace kiss;
using namespace kiss::widget;

InfoBox::InfoBox(QWidget *parent)
  : QTableWidget(parent)
{
  m_state = InfoBox::NONE;
}

void InfoBox::setState(InfoBox::State state)
{
  m_state = state;
}

InfoBox::State InfoBox::state() const
{
  return m_state;
}

void InfoBox::setName(const QString &name)
{
  m_name = name;
}

void InfoBox::setPath(const QString &path)
{
  m_path = path;
}

void InfoBox::setDeps(const QStringList &deps)
{
  m_deps = deps;
}

void InfoBox::refresh()
{
  if(m_state == InfoBox::NONE) {
    setRowCount(0);
    return;
  }
  
  setRowCount(2);
  setItem(0, 0, new QTableWidgetItem("Name"));
  setItem(0, 1, new QTableWidgetItem(m_name));
  setItem(1, 0, new QTableWidgetItem("Path"));
  setItem(1, 1, new QTableWidgetItem(m_path));
}

void InfoBox::showProjectInfo(const QString &name, const QString &path, const QStringList &deps)
{
  setRowCount(3);
  
  setItem(0, 0, new QTableWidgetItem("Name"));
  setItem(1, 0, new QTableWidgetItem("Path"));
  setItem(2, 0, new QTableWidgetItem("Dependencies"));
  
  setItem(0, 1, new QTableWidgetItem(name));
  setItem(1, 1, new QTableWidgetItem(path));
  setItem(2, 1, new QTableWidgetItem(deps.join(", ")));
}
void InfoBox::showFileInfo(const QString &name, const QString &path)
{
  setRowCount(2);
  
  setItem(0, 0, new QTableWidgetItem("Name"));
  setItem(1, 0, new QTableWidgetItem("Path"));
  
  setItem(0, 1, new QTableWidgetItem(name));
  setItem(1, 1, new QTableWidgetItem(path));
}
void InfoBox::showNoInfo()
{
  setRowCount(0);
}
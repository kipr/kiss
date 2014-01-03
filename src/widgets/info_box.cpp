#include "info_box.hpp"
#include <QFileInfo>

using namespace kiss;
using namespace kiss::widget;

InfoBox::InfoBox(QWidget *parent)
  : QTableWidget(parent)
{
}

void InfoBox::showProjectInfo(project::ProjectPtr p)
{
  setRowCount(4);
  setItem(0, 0, new QTableWidgetItem("Name"));
  setItem(1, 0, new QTableWidgetItem("Path"));
  setItem(2, 0, new QTableWidgetItem("Target"));
  setItem(3, 0, new QTableWidgetItem("Dependencies"));
  setItem(0, 1, new QTableWidgetItem(p->name()));
  setItem(1, 1, new QTableWidgetItem(p->location()));
  setItem(2, 1, new QTableWidgetItem(p->target() ? p->target()->displayName() : "No Target"));
  setItem(3, 1, new QTableWidgetItem(p->deps().join(", ")));
}
void InfoBox::showFileInfo(const QString &filePath)
{
  const QFileInfo file(filePath);
  
  setRowCount(2);
  setItem(0, 0, new QTableWidgetItem("Name"));
  setItem(1, 0, new QTableWidgetItem("Path"));
  setItem(0, 1, new QTableWidgetItem(file.fileName()));
  setItem(1, 1, new QTableWidgetItem(file.absoluteFilePath()));
}
void InfoBox::showNoInfo()
{
  setRowCount(0);
}
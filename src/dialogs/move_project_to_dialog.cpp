#include "move_project_to_dialog.hpp"
#include "ui_MoveProjectToDialog.h"
#include "file_utils.hpp"
#include <QDebug>

using namespace kiss::dialog;

MoveProjectToDialog::MoveProjectToDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::MoveProjectToDialog())
{
  ui->setupUi(this);
  connect(ui->browse, SIGNAL(clicked()), this, SLOT(browse()));
}

void MoveProjectToDialog::browse()
{
  qDebug() << "Browse clicked!";
  const QString loc = FileUtils::getExistingDirectory(this, tr("New Location"));
  ui->newLocation->setText(loc);
}

const QString MoveProjectToDialog::newLocation() const
{
  return ui->newLocation->text();
}

const bool MoveProjectToDialog::removeProject() const
{
  return ui->removeProject->isChecked();
}
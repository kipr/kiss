#ifndef _MOVE_PROJECT_TO_DIALOG_
#define _MOVE_PROJECT_TO_DIALOG_

#include <QDialog>

namespace Ui
{
  class MoveProjectToDialog;
}

namespace kiss
{
  namespace dialog
  {
    class MoveProjectToDialog : public QDialog
    {
    Q_OBJECT
    public:
      MoveProjectToDialog(QWidget *parent = 0);
      
      const QString newLocation() const;
      const bool removeProject() const;
      
    private slots:
      void browse();
      
    private:
      Ui::MoveProjectToDialog *ui;
    };
  }
}

#endif
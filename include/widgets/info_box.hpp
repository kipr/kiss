#include <QTableWidget>
#include "project.hpp"

namespace kiss
{
  namespace widget
  {
    class InfoBox : public QTableWidget
    {
    public:
      
      InfoBox(QWidget *parent = 0);
      
      void showProjectInfo(project::ProjectPtr p);
      void showFileInfo(const QString &filePath);
      void showNoInfo();
      
    private:      
      QString m_name;
      QString m_path;
      QStringList m_deps;
    };
  }
}
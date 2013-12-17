#include <QTableWidget>

namespace kiss
{
  namespace widget
  {
    class InfoBox : public QTableWidget
    {
    public:
      enum State
      {
        PROJECT,
        FILE,
        NONE
      };
      
      InfoBox(QWidget *parent = 0);
      
      void setState(InfoBox::State state);
      InfoBox::State state() const;
      
      void setName(const QString &name);
      void setPath(const QString &path);
      
      void setDeps(const QStringList &deps);
      
      void refresh();
      
      void showProjectInfo(const QString &name, const QString &path, const QStringList &deps);
      void showFileInfo(const QString &name, const QString &path);
      void showNoInfo();
      
    private:
      InfoBox::State m_state;
      
      QString m_name;
      QString m_path;
      QStringList m_deps;
    };
  }
}
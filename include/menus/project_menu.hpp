#ifndef _PROJECT_MENU_HPP_
#define _PROJECT_MENU_HPP_

#include "activatable.hpp"
#include "concrete_menuable.hpp"

namespace kiss
{  
  namespace menu
  {
    class ProjectMenu : public ConcreteMenuable, public ActivatableObject
    {
    Q_OBJECT
    public:
      ProjectMenu();
      void refresh();

      static QString menuName();
      
    public slots:
      void update();

    protected:
      void activated();
      void deactivated();
      
    private:
      Node *m_projectMenu;
      Node *compileNode;
      Node *downloadNode;
      Node *runNode;
    };
  }
}

#endif

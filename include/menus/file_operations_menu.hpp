#ifndef _FILEOPERATIONS_MENU_HPP_
#define _FILEOPERATIONS_MENU_HPP_

#include "activatable.hpp"
#include "concrete_menuable.hpp"

namespace kiss
{
  namespace menu
  {
    class FileOperationsMenu : public ConcreteMenuable, public ActivatableObject
    {
    Q_OBJECT
    public:
      FileOperationsMenu();

      static QString menuName();
    };
  }
}

#endif

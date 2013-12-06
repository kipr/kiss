#ifndef _PROJECT_MENU_HPP_
#define _SOURCE_FILE_MENU_HPP_

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

			static QString menuName();

		protected:
			void activated();
			void deactivated();
      
    private:
      Node *m_projectMenu;
		};
	}
}

#endif

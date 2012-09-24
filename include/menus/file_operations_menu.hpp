#ifndef _FILEOPERATIONS_MENU_HPP_
#define _FILEOPERATIONS_MENU_HPP_

#include "activatable.hpp"
#include "concrete_menuable.hpp"

namespace Kiss
{
	namespace Menu
	{
		class FileOperationsMenu : public ConcreteMenuable, public ActivatableObject
		{
		public:
			FileOperationsMenu();

			static QString menuName();
		};
	}
}

#endif

#ifndef _FILEOPERATIONSMENU_H_
#define _FILEOPERATIONSMENU_H_

#include "Activatable.h"
#include "ConcreteMenuable.h"

class FileOperationsMenu : public ConcreteMenuable, public ActivatableObject
{
public:
	FileOperationsMenu();
	
	static QString menuName();
};

#endif

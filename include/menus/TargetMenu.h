#ifndef _TARGETMENU_H_
#define _TARGETMENU_H_

#include "MenuManager.h"
#include "Singleton.h"
#include "Activatable.h"
#include "SourceFile.h"
#include "ConcreteMenuable.h"

#include <QMultiMap>

class TargetMenu : public ConcreteMenuable, public Singleton<TargetMenu>, public ActivatableObject
{
Q_OBJECT
public:
	TargetMenu();
	void refresh();
	
	MenuNode* additional();
	
	MenuNodeList manuals();
	MenuNodeList manuals(const QString& target);
	
	static QString menuName();
	
protected:
	void activated();
	void deactivated();
	
protected slots:
	virtual void triggered();
	
private:
	QMultiMap<QString, MenuNode*> m_manuals;
	QString m_target;
	
	MenuNode* m_additional;

	MenuNode* compileNode;
	MenuNode* downloadNode;
	MenuNode* simulateNode;
	MenuNode* runNode;
	MenuNode* stopNode;
	MenuNode* debugNode;
};

#endif
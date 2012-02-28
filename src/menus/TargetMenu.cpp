#include "TargetMenu.h"

#include "Target.h"
#include "TargetManager.h"

#include "ResourceHelper.h"
#include "Documentation.h"
#include "MainWindow.h"

TargetMenu::TargetMenu() : ConcreteMenuable(menuName()), m_additional(MenuNode::insertionPoint())
{
	// Put manuals in "Target"
	MenuNode* targetMenu = new MenuNode("Target");
	
	targetMenu->children.append(compileNode = node(activeAction("bricks", "Compile", QKeySequence("Alt+C"), this, "compile")));
	targetMenu->children.append(downloadNode = node(activeAction("ruby_blue", "Download", QKeySequence("Alt+D"), this, "download")));
	targetMenu->children.append(simulateNode = node(activeAction("application_go", "Simulate", QKeySequence("Alt+S"), this, "simulate")));
	targetMenu->children.append(runNode = node(activeAction("arrow_right", "Run", QKeySequence("Alt+R"), this, "run")));
	targetMenu->children.append(stopNode = node(activeAction("stop.png", "Stop", QKeySequence("Alt+X"), this, "stop")));
	targetMenu->children.append(debugNode = node(activeAction("bug", "Debug", QKeySequence("Alt+G"), this, "debug")));
	
	compileNode->activeControl = downloadNode->activeControl = true;
	simulateNode->activeControl = runNode->activeControl = true;
	stopNode->activeControl = debugNode->activeControl = true;
	
	m_toolbar.append(targetMenu->children);
	targetMenu->children.append(MenuNode::separator());
	targetMenu->children.append(node(activeAction("", "Change Target", QKeySequence("Alt+T"), this, "changeTarget")));
	targetMenu->children.append(node(activeAction("link", "Choose Port", QKeySequence("Alt+P"), this, "choosePort")));
	targetMenu->children.append(MenuNode::separator());
	//targetMenu->children.append(m_additional);
	
	m_actions.append(targetMenu);
	
	finish();
}

MenuNode* TargetMenu::additional()
{
	return m_additional;
}

void TargetMenu::refresh()
{
	if(!isActive()) return;
	Target* target = qobject_cast<SourceFile*>(active())->target();
	
	compileNode->rawAction->setEnabled(target->hasCompile());
	downloadNode->rawAction->setEnabled(target->hasDownload());
	simulateNode->rawAction->setEnabled(target->hasSimulate());
	runNode->rawAction->setEnabled(target->hasRun());
	stopNode->rawAction->setEnabled(target->hasStop());
	debugNode->rawAction->setEnabled(target->hasDebug());
	
	menuManager()->refreshToolbar();
}

void TargetMenu::activated()
{
	menuManager()->addActivation(this);
	
	refresh();
}

void TargetMenu::deactivated()
{
	compileNode->rawAction->setEnabled(false);
	downloadNode->rawAction->setEnabled(false);
	simulateNode->rawAction->setEnabled(false);
	runNode->rawAction->setEnabled(false);
	stopNode->rawAction->setEnabled(false);
	debugNode->rawAction->setEnabled(false);
	
	menuManager()->removeActivation(this);
}

MenuNodeList TargetMenu::manuals()
{
	return m_manuals.values();
}

MenuNodeList TargetMenu::manuals(const QString& target)
{
	return m_manuals.values(target);
}

void TargetMenu::triggered()
{
	QAction* _ = (QAction*)sender();
}

QString TargetMenu::menuName()
{
	return "Target";
}

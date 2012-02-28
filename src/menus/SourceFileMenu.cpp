#include "SourceFileMenu.h"
#include "MainWindowMenu.h"

#include "ResourceHelper.h"
#include "MainWindow.h"

#include <QIcon>
#include <QDebug>

SourceFileMenu::SourceFileMenu(MainWindow* mainWindow) : ConcreteMenuable(menuName())
{
	const QIcon disk(ResourceHelper::ref().lookup("disk"));
	
	MenuNode* fileOp = dynamic_cast<MainWindowMenu*>(mainWindow->menuable(MainWindowMenu::menuName()))->fileOperations();
	fileOp->children.append(node(activeAction(disk, "Save", QKeySequence::Save, this, "save")));
	fileOp->children.append(node(activeAction(disk, "Save As...", QKeySequence::SaveAs, this, "saveAs")));
	fileOp->children.append(node(activeAction("print", "Print", QKeySequence::Print, this, "print")));
	
	MenuNode* copy = node(activeAction("copy", "Copy", QKeySequence::Copy, this, "copy"));
	MenuNode* cut = node(activeAction("cut", "Cut", QKeySequence::Cut, this, "cut"));
	MenuNode* paste = node(activeAction("paste", "Paste", QKeySequence::Paste, this, "paste"));
	
	m_edit.append(copy);
	m_edit.append(cut);
	m_edit.append(paste);
	
	m_edit.append(MenuNode::separator());
	m_edit.append(undo = node(activeAction("arrow_undo", "Undo", QKeySequence::Undo, this, "undo")));
	m_edit.append(redo = node(activeAction("arrow_redo", "Redo", QKeySequence::Redo, this, "redo")));
	m_edit.append(MenuNode::separator());
	m_edit.append(node(activeAction("", "Zoom In", QKeySequence::ZoomIn, this, "zoomIn")));
	m_edit.append(node(activeAction("", "Zoom Out", QKeySequence::ZoomOut, this, "zoomOut")));
	m_edit.append(node(activeAction("", "Reset Zoom", QKeySequence("Ctrl+0"), this, "zoomReset")));
	m_edit.append(MenuNode::separator());
	m_edit.append(node(activeAction("find", "Find", QKeySequence::Find, this, "find")));
	
	// undo->activeControl = redo->activeControl = true;
	
	MenuNode* source = new MenuNode("Source");
	
	source->children.append(node(activeAction("", "Indent All", QKeySequence("Ctrl+I"), this, "indentAll")));
	source->children.append(MenuNode::separator());
	source->children.append(breakpoint = node(activeCheckedAction(QIcon(), "Toggle Breakpoint", QKeySequence("Ctrl+Shift+B"), this, "toggleBreakpoint")));
	source->children.append(node(activeAction("", "Clear All Breakpoints", QKeySequence("Ctrl+Alt+Shift+B"), this, "clearBreakpoints")));
	
	m_actions.append(source);
	
	m_toolbar.append(copy);
	m_toolbar.append(cut);
	m_toolbar.append(paste);
	m_toolbar.append(MenuNode::separator());
	
	finish();
}

MenuNode* SourceFileMenu::breakpointToggle() const
{
	return breakpoint;
}

void SourceFileMenu::activated()
{
	menuManager()->addActivation(this);
}

void SourceFileMenu::deactivated()
{
	menuManager()->removeActivation(this);
}

void SourceFileMenu::triggered()
{
	if(!isActive()) return;
	SourceFile* source = qobject_cast<SourceFile*>(active());
	
	QAction* _ = (QAction*)sender();
}

#if 0

void SourceFile::addActionsFile(QMenu* file) 
{
	file->addAction(actionSave);
	file->addAction(actionSaveAs);
	file->addSeparator();
	file->addAction(actionPrint);
	file->addSeparator();
	file->addAction(actionMakeTemplate);
}

void SourceFile::addActionsEdit(QMenu* edit)
{
	edit->addAction(actionCopy);
	edit->addAction(actionCut);
	edit->addAction(actionPaste);
	edit->addSeparator();
	edit->addAction(actionUndo);
	edit->addAction(actionRedo);
}

void SourceFile::addActionsHelp(QMenu* help) 
{
	const QMap<QString, QString>& manuals = m_target.targetManualPaths();
	foreach(const QString& manual, manuals.keys()) {
		if(!QFileInfo(manuals[manual]).exists()) continue;
		QAction* action = help->addAction(QIcon(":/shortcuts/target/icon_set/icons/report.png"), manual);
		action->setData(manuals[manual]);
		connect(action, SIGNAL(triggered()), this, SLOT(openManual()));
	}
}

void SourceFile::addOtherActions(QMenuBar* menuBar)
{
	QMenu* source = menuBar->addMenu(tr("Source"));
	QMenu* target = menuBar->addMenu(tr("Target"));
	source->addAction(actionZoomIn);
	source->addAction(actionZoomOut);
	source->addAction(actionResetZoomLevel);
	if(m_target.cStyleBlocks()) {
		source->addSeparator();
		source->addAction(actionIndentAll);
	}
	source->addSeparator();
	source->addAction(actionFind);
	if(m_target.hasCompile()) target->addAction(actionCompile);
	if(m_target.hasDownload()) target->addAction(actionDownload);
	if(m_target.hasSimulate()) target->addAction(actionSimulate);
	if(m_target.hasRun()) target->addAction(actionRun);
	if(m_target.hasStop()) target->addAction(actionStop);
	if(m_target.hasDebug()) {
		target->addAction(actionDebug);
		source->addSeparator();
		source->addAction(actionToggleBreakpoint);
		source->addAction(actionClearBreakpoints);
	}
	target->addSeparator();
	target->addAction(actionChangeTarget);
	target->addAction(actionChoosePort);
	target->addSeparator();
	const QList<QAction*>& actionList = m_target.actionList();
	for(int i = 0; i < actionList.size(); ++i) target->insertAction(0, actionList[i]);
	if(actionList.size() > 0) target->addSeparator();
	bool hasExtra = false;
	if(m_target.hasScreenGrab()) {
		target->addAction(actionScreenGrab);
		hasExtra = true;
	}
	if(m_target.hasRequestFile()) {
		target->addAction(actionRequestFile);
		hasExtra = true;
	}
	if(hasExtra) target->addSeparator();
	addActionsHelp(target);
}

void SourceFile::addToolbarActions(QToolBar* toolbar) 
{
	toolbar->addAction(actionSave);
	toolbar->addSeparator();
	toolbar->addAction(actionCopy);
	toolbar->addAction(actionCut);
	toolbar->addAction(actionPaste);
	toolbar->addAction(actionFind);
	toolbar->addSeparator();
	
	if(m_target.hasCompile()) toolbar->addAction(actionCompile);
	if(m_target.hasDownload()) toolbar->addAction(actionDownload);
	if(m_target.hasSimulate()) toolbar->addAction(actionSimulate);
	if(m_target.hasRun()) toolbar->addAction(actionRun);
	if(m_target.hasStop()) toolbar->addAction(actionStop);
	if(m_target.hasDebug()) toolbar->addAction(actionDebug);
}
#endif

QString SourceFileMenu::menuName()
{
	return "Source";
}
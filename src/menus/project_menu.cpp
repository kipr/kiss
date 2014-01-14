#include "project_menu.hpp"

#include "main_window.hpp"
#include "communication_manager.hpp"
#include "system_utils.hpp"

using namespace kiss::menu;

ProjectMenu::ProjectMenu()
  : ConcreteMenuable(menuName())
{
  m_projectMenu = new Node(menuName());
  
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("page_white_add.png"),
    tr("Add New File..."), QKeySequence::UnknownKey, this, "activeProjectAddNew")));
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("page_white_add.png"),
    tr("Add Existing Files..."), QKeySequence::UnknownKey, this, "activeProjectAddExisting")));
  m_projectMenu->children.append(node(activeAction(ResourceHelper::ref().icon("folder_add.png"),
    tr("Add Folder"), QKeySequence::UnknownKey, this, "activeProjectAddFolder")));
  m_projectMenu->children.append(Node::separator());
  m_projectMenu->children.append(downloadNode = node(activeAction("ruby_blue",
    tr("Download"), QKeySequence("Alt+D"), this, "activeProjectDownload")));
  m_projectMenu->children.append(compileNode = node(activeAction("bricks",
    tr("Compile"), QKeySequence("Alt+C"), this, "activeProjectCompile")));
  m_projectMenu->children.append(runNode = node(activeAction("arrow_right",
    tr("Run"), QKeySequence("Alt+R"), this, "activeProjectRun")));
  m_projectMenu->children.append(node(activeAction("computer",
    tr("Change Target"), QKeySequence("Alt+T"), this, "activeProjectChangeTarget")));
  m_projectMenu->children.append(Node::separator());
  m_projectMenu->children.append(node(activeAction("brick_go",
    tr("Move Project To..."), QKeySequence::UnknownKey, this, "activeProjectMoveTo")));
  m_projectMenu->children.append(node(activeAction("mouse",
    tr("View in File Browser"), QKeySequence::UnknownKey, this, "activeProjectFileBrowser")));
  m_projectMenu->children.append(node(activeAction("folder_wrench.png",
    tr("Project Settings"), QKeySequence::UnknownKey, this, "activeProjectOpenSettings")));
  m_projectMenu->children.append(Node::separator());
  m_projectMenu->children.append(node(activeAction("folder.png",
    tr("Close Project"), QKeySequence::UnknownKey, this, "activeProjectClose")));
  m_projectMenu->children.append(node(activeAction("bin_closed.png", SystemUtils::supportsMoveToTrash() ?
    tr("Move to Trash") : tr("Permanently Delete"), QKeySequence::UnknownKey, this, "activeProjectDelete")));
    
  m_actions.append(m_projectMenu);
  m_toolbar.append(compileNode);
  m_toolbar.append(runNode);
  
  compileNode->hideOnDisable = false;
  downloadNode->hideOnDisable = false;
  runNode->hideOnDisable = false;
}

void ProjectMenu::refresh()
{
  if(!isActive()) return;
  menuManager()->refreshToolbar();
}

void ProjectMenu::update()
{
  const bool enabled = target::CommunicationManager::ref().isIdle();
  compileNode->rawAction->setEnabled(enabled);
  downloadNode->rawAction->setEnabled(enabled);
  runNode->rawAction->setEnabled(enabled);
  
  {
    kiss::widget::MainWindow *window = qobject_cast<kiss::widget::MainWindow *>(active());
    project::Manager *manager = window->projectManager();
    const project::ProjectPtr project = manager->activeProject();
    QString extra;
    if(!project.isNull() && manager->projects().size() != 1) {
      const QString &name = project->name();
      const static int maxSize = 8;
      extra += " " + name.left(maxSize);
      if(name.size() > maxSize) extra += "\u2026";
    }
    compileNode->rawAction->setText(tr("Compile%1").arg(extra));
    downloadNode->rawAction->setText(tr("Download%1").arg(extra));
    runNode->rawAction->setText(tr("Run%1").arg(extra));
  }
  
  refresh();
}

void ProjectMenu::activated()
{
  ActivatableObject::activated();
  menuManager()->addActivation(this);
  refresh();
}

void ProjectMenu::deactivated()
{
  menuManager()->removeActivation(this);
  ActivatableObject::deactivated();
}

QString ProjectMenu::menuName()
{
  return "Project";
}
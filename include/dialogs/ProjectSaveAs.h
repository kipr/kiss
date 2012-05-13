#ifndef _PROJECTSAVEAS_H_
#define _PROJECTSAVEAS_H_

#include "ui_ProjectSaveAs.h"
#include "ProjectsModel.h"

class MainWindow;
class ProjectManager;

class ProjectSaveAs : public QDialog, public Ui::ProjectSaveAs
{
Q_OBJECT
public:
	ProjectSaveAs(MainWindow* mainWindow);
	
	void setDefaultProject(Project* project);
	
	Project* activeProject() const;
	const TinyNode* parent() const;
	QString fileName() const;
	
private slots:
	void on_ui_filename_textChanged(const QString& text);
	void selectionChanged(const QModelIndex& index);
	
private:
	void setActiveProject(Project* project);
	void setParentNode(const TinyNode* parent);
	
	void updateActiveLabel();
	
	MainWindow* m_mainWindow;
	
	Project* m_active;
	const TinyNode* m_parent;
	
	QStringList m_exts;
};

#endif

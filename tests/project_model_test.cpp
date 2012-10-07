#include <QApplication>
#include <QTreeView>

#include "project_model.hpp"
#include "project.hpp"

using namespace Kiss;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	Kiss::Project::Model model;
	model.addProject(Project::Project::load("/Volumes/Eve/Development/kipr/kiss"));
	// model.addRootPath("/Volumes/Eve/Development/kipr/kiss");
	model.addRootPath("/Volumes/Kevin");
	
	QTreeView tree;
	tree.setModel(&model);
	
	tree.show();
	tree.raise();
	
	return app.exec();
}
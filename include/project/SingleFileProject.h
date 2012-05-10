#ifndef _SINGLEFILEPROJECTHELPER_H_
#define _SINGLEFILEPROJECTHELPER_H_

#include <QString>

class Project;
class ArchiveWriter;

class SingleFileProjectHelper
{
public:
	SingleFileProjectHelper(const QString& file);
	
	const QDir& root() const;
	
	Project* project();
	ArchiveWriter* writer();
private:
	Project* m_project;
	ArchiveWriter* m_writer;
};

#endif

#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <QString>
#include <QIODevice>

class Project
{
public:	
	static Project* load(QIODevice* in);
protected:
private:
	
	QString m_name;
};

#endif

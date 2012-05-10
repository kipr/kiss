#include "CommandChain.h"

#include <QElapsedTimer>
#include <QDebug>
#include <QStringList>
#include <QProcess>
#include <iostream>

#define GCC_EXECUTABLE "gcc"

int main(int argc, char* argv[])
{
	CommandChain commandChain(4);
	commandChain.add(new TimeSegment(1000));
	commandChain.add(new TimeSegment(1500));
	commandChain.add(new QProcessSegment(GCC_EXECUTABLE, QStringList() << "--help")); 
	commandChain.add(new TimeSegment(2000));
	commandChain.add(new TimeSegment(700));
	commandChain.add(new TimeSegment(1000));
	if(!commandChain.execute()) {
		qDebug() << "There was an error executing the chain!";
	}
	QIODevice* out = commandChain.chainSession()->out();
	QIODevice* err = commandChain.chainSession()->err();
	
	qDebug() << "stdout:" << out->readAll();
	qCritical() << "stderr:" << err->readAll();
}
/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "TargetManager.h"
#include "Target.h"
#include "Kiss.h"

#include <unistd.h>
#include <QString>
#include <iostream>

using namespace std;

void warning() { cout << "KIPR is not responsible for damages caused by this tool." << endl; }


void usage(char* location) {
	cout << "Usage: %s [target_name] [port|none] [action] [action params]" << endl;
	warning();
	exit(1);
}

QString boolRet(bool has) { return has ? "Yes" : "No"; }

int main (int argc, char* argv[])
{
	if(argc < 4) usage(argv[0]);
	
	QString targetName(argv[1]);
	QString targetPath(TargetManager::ref().targetPath(targetName) + "/" + targetName + "." + TARGET_EXT);
	Target target;
	if(target.setTargetFile(targetPath)) {
		cerr << "Unable to load " << targetName.toStdString() << " from " << targetPath << endl;
		exit(1);
	}
	
	QString port(argv[2]);
	if(port != "none") target.setPort(port);
	
	QString action(argv[3]);
	if(action == "list") {
		cout << "Compile:  " << boolRet(target.hasCompile()).toStdString() << endl;
		cout << "Download: " << boolRet(target.hasDownload()).toStdString() << endl;
		cout << "Run:      " << boolRet(target.hasRun()).toStdString() << endl;
		cout << "Stop:     " << boolRet(target.hasStop()).toStdString() << endl;
	} else if(action == "download") {
		if(argc < 5) cerr << "Download expects file name as param" << endl;
		else cout << "Success? " << boolRet(target.download(argv[4])) << endl;
	} else if(action == "compile") {
		if(argc < 5) cerr << "Compile expects file name as param" << endl;
		else cout << "Success? " << boolRet(target.compile(argv[4])) << endl;
	} else if(action == "run") {
		if(argc < 5) cerr << "Run expects file name as param" << endl;
		else cout << "Success? " << boolRet(target.run(argv[4])) << endl;
	} else if(action == "stop") {
		target.stop();
	}
	
	return 0;
}
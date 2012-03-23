#include <TinyArchive.h>
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#define LIST_COMMAND "list"
#define CREATE_COMMAND "create"
#define EXTRACT_COMMAND "extract"

void usage(const char* location)
{
	cout << location << " "
		<< LIST_COMMAND << "|"
		<< CREATE_COMMAND << "|"
		<< EXTRACT_COMMAND << " tiny.archive [parameters]" << endl;
}

int main(int argc, char* argv[])
{
	if(argc == 1 || argc == 2) {
		cout << "Error: Too few arguments" << endl;
		usage(argv[0]);
		return 1;
	}
	
	char* command = argv[1];
	char* archive = argv[2];
	
	TinyArchive* tinyArchive = 0;
	
	if(!strcmp(command, LIST_COMMAND)) {
		ifstream ifs;
		ifs.open(archive, ifstream::in);
		if(!ifs.is_open()) {
			cout << "Error: Unable to open " << archive << " for reading";
			return 1;
		}
		tinyArchive = TinyArchive::read(ifs);
		if(!tinyArchive) {
			cout << "Tiny Archive encountered an error while reading " << archive;
			return 1;
		}
		ifs.close();
		TinyArchive::iterator it = tinyArchive->begin();
		for(; it != tinyArchive->end(); ++it) {
			cout << (*it).path << endl;
		}
		delete tinyArchive;
	} else if(!strcmp(command, CREATE_COMMAND)) {
		tinyArchive = new TinyArchive();
		for(int i = 3; i < argc; ++i) {
			ifstream ifs;
			ifs.open(argv[i], ifstream::in);
			if(!ifs.is_open()) {
				cout << "Error: Unable to open " << argv[i] << " for reading";
				return 1;
			}
			char* test = "test";
			tinyArchive->push_back(ArchiveEntry(argv[i], test, 4));
			ifs.close();
		}
		
		ofstream ofs;
		ofs.open(archive, ifstream::out);
		tinyArchive->write(ofs);
		ofs.close();
		
		delete tinyArchive;
	} else if(!strcmp(command, EXTRACT_COMMAND)) {
	} else {
		cout << "Error: Unrecognized command \"" << command << "\"" << endl;
		usage(argv[0]);
		return 1;
	}
	
	return 0;
}
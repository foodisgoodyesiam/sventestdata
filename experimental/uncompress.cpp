#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

const char* programName;

int main(int argNum, const char** args) {
	programName = args[0];
	vector<string> lines;
	ifstream in("uniquefortesting.txt", ios::in | ios::binary);
	if (in.good()) {
		string line;
		while (getline(in, line)) {
			lines.push_back(line);
		}
	} else {
		fprintf(stderr, "%s: could not open uniquefortesting: %s\n", programName, strerror(errno));
		return 1;
	}
	in.close();
	// TODO: have it read a file containing line numbers (a binary file, why not)
	// TODO: move this to same folder in jashergit as maxlinelength, so the git submodule can be relative (if I'm understanding that concept right...)
}

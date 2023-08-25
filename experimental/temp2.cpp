#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdint>

using namespace std;

const char* programName;

int main(int argNum, const char** args) {
	programName = args[0];
	vector<string> lines;
	ifstream in("/home/sven/source/maxlinelength/uniquefortesting.txt", ios::in | ios::binary);
	unordered_map<string, int> lineMap;
	if (in.good()) {
		string line;
		int i = 0;
		while (getline(in, line)) {
			lineMap[line] = i;
			i++;
		}
	} else {
		fprintf(stderr, "%s: could not open uniquefortesting: %s\n", programName, strerror(errno));
		return 1;
	}
/*	for (auto a : lineMap)
		cout << a.second << ' ' << a.first << '\n';
	return 0;*/
	in.close();
	in.open("/home/sven/obfuscateddatafortesting.txt", ios::in | ios::binary);
/*				auto found = lineMap.find(line);
			if (found == lineMap.end())
				fprintf(stderr, "%s: line %s not found\n", programName, line.c_str());
			else {
//				cout << found->second << ' ' << found->first << '\n';

			}*/

	if (in.good()) {
		string line;
		while (getline(in, line)) {
			lines.push_back(line);
		}
	} else {
		fprintf(stderr, "%s: could not open obfuscateddatafortesting: %s\n", programName, strerror(errno));
		return 1;
	}
	in.close();
	cerr << "Found " << lines.size() << " lines\n";

	uint8_t num = 1;
	vector<uint32_t> numsToWrite;
	for (size_t i=0; i<lines.size(); i++) {
		if (i!=lines.size()-1 && !lines[i].compare(lines[i+1]))
			num++;
		else {
            uint32_t toWrite = lineMap[lines[i]]<<2 | num;
            //fwrite(&twoWrite, 4, 1, out);
            numsToWrite.push_back(toWrite);
			//cout << num << '\t' << lines[i] << '\n';
			num = 1;
		}
	}
    FILE* out = fopen("binaryfortesting", "wb");
	if (out == NULL) {
        fprintf(stderr, "%s: could not open binaryfortesting: %s\n", programName, strerror(errno));
        return 1;
	}
	fwrite(&numsToWrite[0], 4, numsToWrite.size(), out);
	fclose(out);
	// TODO: have it read a file containing line numbers (a binary file, why not)
	// TODO: move this to same folder in jashergit as maxlinelength, so the git submodule can be relative (if I'm understanding that concept right...)
}

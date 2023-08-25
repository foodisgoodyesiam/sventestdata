#include <iostream>
#include <unordered_set>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdio>

using namespace std;

static const char* programName;

int main(int argNum, const char** args) {
    programName = args[0];
    unordered_set<string> lines;
    ifstream in("obfuscateddatafortesting.txt", ios::in | ios::binary);
    if (!__builtin_expect(in.good(), true)) {
        fprintf(stderr, "%s: could not open obfuscateddatafortesting.txt: %s\n", programName, strerror(errno));
        exit(1);
    }
    ofstream out("uniquefortesting.txt", ios::out | ios::binary);
        if (!__builtin_expect(out.good(), true)) {
        fprintf(stderr, "%s: could not open uniquefortesting for writing.txt: %s\n", programName, strerror(errno));
        exit(1);
    }
    string line;
    while (getline(in, line)) {
        if (lines.find(line)==lines.end()) {
            lines.insert(line);
            out << line << '\n';
        }
    }
    in.close();
    out.close();
}

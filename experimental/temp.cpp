#include <stdio.h>

const char* programName;

int main(int argNum, const char** args) {
	programName = args[0];
	FILE* in = fopen("/home/sven/obfuscateddatafortesting.txt", "rb");
	char* buf = NULL;
	size_t length = 0;
	size_t numRead;
	do {
		numRead = getline(&buf, &numRead);
		
	} while (numRead > 0);
}

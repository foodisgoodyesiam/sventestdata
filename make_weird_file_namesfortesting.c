#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define TEMPLATE_NAME "weird_file_names_fortesting/weird_name_a.txt"
#define NAME_BUF_LEN (__builtin_strlen(TEMPLATE_NAME)+1)

static const char* programName;

int main(int argNum, const char** args) {
	programName = args[0];
	char name_buf[NAME_BUF_LEN] = TEMPLATE_NAME;
	name_buf[NAME_BUF_LEN-1] = '\0';
	for (unsigned short c=1; c<256; c++) {
		if (c=='/')
			continue;
		name_buf[strlen("weird_file_names_fortesting/weird_name_")] = c;
		int fd = open(name_buf, O_WRONLY|O_CREAT|O_NOCTTY|O_NONBLOCK, 0666);
		if (fd>0) {
			write(fd, name_buf, NAME_BUF_LEN);
			close(fd);
		} else {
			fprintf(stderr, "%s: could not open %s: %s", programName, name_buf, strerror(errno));
			return 1;
		}
	}
	return 0;
}

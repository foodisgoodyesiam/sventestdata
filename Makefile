SHELL::=/bin/bash

allutffiles ::= all_utf8_fortesting.txt \
	assigned_utf8_fortesting.txt \
	all_utf16le_fortesting.txt \
	assigned_utf16le_fortesting.txt \
	all_utf16be_fortesting.txt \
	assigned_utf16be_fortesting.txt \
	all_utf32le_fortesting.txt \
	all_utf32be_fortesting.txt \
	assigned_utf32le_fortesting.txt \
	assigned_utf32be_fotesting.txt

all: sequentialnumsfortesting.txt \
	allsamelinefortesting.txt \
	wordlist-mit-10000.txt \
	wordlist-mit-100000.txt \
	obfuscateddatafortesting.txt \
	uniquefortesting.txt \
	$(allutffiles) \
	dirs_fortesting \
	pipe_fortesting

os::=$(shell uname -o | tr '/' '_')

CFLAGS ?= -O3

ifeq ($(os),Darwin)
	CFLAGS ::= $(CFLAGS) -I/usr/local/include -I/opt/homebrew/include
	LDFLAGS ::= $(LDFLAGS) -L/opt/homebrew/lib
endif
LDLIBS ::= -lunistring

sequentialnumsfortesting.txt:
#	# winner, 0.199s
	seq 1 10000000 > $@
#	makes sense. It manipulates string buffers directly rather than number representation (which need division...)
#	# 1.952s
#	nums 1-10000000 >$@
#	# 4m1.249s
#	for ((i=1;i<=10000000;i++)); do echo $$i; done >$@

obfuscateddatafortesting.txt:
	7z x compressed.7z

allsamelinefortesting.txt:
	yes thisisastring | head -1000000 > $@

wordlist-mit-10000.txt:
	wget -O $@ https://www.mit.edu/~ecprice/wordlist.10000

wordlist-mit-100000.txt:
	wget -O $@ https://www.mit.edu/~ecprice/wordlist.100000

uniquefortesting.txt: makeuniquefortesting obfuscateddatafortesting.txt
	./makeuniquefortesting


$(allutffiles): makeutf8fortesting
	./makeutf8fortesting

makeuniquefortesting: makeuniquefortesting.cpp

makeutf8fortesting: makeutf8fortesting.c

dirs_fortesting:
	./make_dirs_for_isempty.sh

pipe_fortesting:
	mkfifo $@

#TODO: all unicode code points
#TODO: all valid unicode code points

#TODO: random or pseudo-random stream


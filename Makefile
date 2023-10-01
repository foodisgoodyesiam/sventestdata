SHELL::=/bin/bash

all: sequentialnumsfortesting.txt \
	allsamelinefortesting.txt \
	wordlist-mit-10000.txt \
	wordlist-mit-100000.txt \
	obfuscateddatafortesting.txt \
	uniquefortesting.txt \
	allutf8fortesting.txt \
	assignedutf8fortesting.txt

os::=$(shell uname -o | tr '/' '_')

CFLAGS ?= -O3

ifeq ($(os),Darwin)
	CFLAGS ::= $(CFLAGS) -I/usr/local/include -I/opt/homebrew/include
	Lunistring ::= -lunistring -L/opt/homebrew/lib
else
	Lunistring ::= -lunistring
endif
LDFLAGS ::= $(LDFLAGS) $(Lunistring)

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

allutf8fortesting.txt assignedutf8fortesting.txt: makeutf8fortesting
	./makeutf8fortesting

makeuniquefortesting: makeuniquefortesting.cpp

makeutf8fortesting: makeutf8fortesting.c

#TODO: all unicode code points
#TODO: all valid unicode code points

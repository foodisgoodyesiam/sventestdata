SHELL::=/bin/bash

all: sequentialnumsfortesting.txt \
	allsamelinefortesting.txt \
	wordlist-mit-10000.txt \
	wordlist-mit-100000.txt \
	obfuscateddatafortesting.txt

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

#TODO: all unicode code points
#TODO: all valid unicode code points

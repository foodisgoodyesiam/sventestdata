SHELL::=/bin/bash

all: sequentialnumsfortesting.txt

sequentialnumsfortesting.txt:
#	# winner, 0.199s
	seq 1 10000000 > $@
#	makes sense. It manipulates string buffers directly rather than number representation (which need division...)
#	# 1.952s
#	nums 1-10000000 >$@
#	# 4m1.249s
#	for ((i=1;i<=10000000;i++)); do echo $$i; done >$@



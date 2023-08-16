#!/usr/bin/env bash

function error() {
	>&2 echo "$0: $1"
}

#   move to dir this script lives in
cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null

n=sequentialnumsfortesting.txt

if [[ -f "$n" ]]; then
	echo "$n: already exists"
elif [[ -e "$n" ]]; then
	error "$n: exists and is not a file?"
fi

#		this version: 4m1.249s
#for ((i=1;i<=10000000;i++)); do
#	echo $i
#done >$n

#		this version: 1.952s
#nums 1-10000000 >$n

#		this is the speed winner: 0.199s
#		makes sense. It manipulates string buffers directly rather than number representation (which need division...)
seq 1 10000000 >$n || error "error in seq. did you fill up your hard drive?"

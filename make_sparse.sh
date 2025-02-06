#!/usr/bin/env bash

# name of 1st output file (the one without the holes punched)
out1=nonsparse_fortesting.txt
# name of 2nd output file (the one with holes)
out2=sparse_fortesting.txt
# number of leading non-null bytes
num_lead=4097
# length of null bytes divided by 32
num_empty_div_32=131072
# number of trailing non-null bytes
num_tail=9000

for ((x=0; x<num_lead; x++)); do
	echo -n "l"
done > "$out1"
for ((x=0; x<num_empty_div_32; x++)); do
	echo -en "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
done >> "$out1"
for ((x=0; x<num_tail; x++)); do
	echo -n "t"
done >> "$out1"
cp "$out1" "$out2" || exit 1
fallocate -d "$out2" || exit 1

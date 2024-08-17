#!/usr/bin/bash
mkdir dirs_fortesting && cd dirs_fortesting
mkdir contains_only_dirs && cd contains_only_dirs
for ((x=0; x<10; x++)); do
	for ((y=0; y<100; y++)); do
		for ((z=0; z<x; z++)); do
			echo -en "$x/stuff_$y/yep_$z\0"
		done
	done
done | xargs -0 mkdir -p
cd ..
cp -r contains_only_dirs contains_a_file_nested || exit 1
touch contains_a_file_nested/7/stuff_50/yep_4/file || exit 1
cp -r contains_only_dirs deeply_nested_file
deep=deeply_nested_file/0/1/2/3/4/5/6/7/8/9/10/11/12/13/14/15/16/17/18/19/20/21/22/23/24/25/26/27/28/29/30/31/32/33/34/35/36/37/38/39/40/41/42/43/44/45/46/47/48/49/50/51/52/53/54/55/56/57/58/59/60/61/62/63/64/65/66/67/68/69/70/71/72/73/74/75/76/77/78/79/80/81/82/83/84/85/86/87
mkdir -p $deep
touch $deep/f
cp -r contains_only_dirs contains_recursive_symlink
ln -s .. contains_recursive_symlink/4/stuff_41/yep_2/recursive_symlink

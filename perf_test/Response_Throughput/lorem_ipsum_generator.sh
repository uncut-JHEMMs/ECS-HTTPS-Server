#!/bin/bash

#Generates data files to be used for throughput testing.
count=3

while getopts "c:h" opt
do
    case $opt in
	c) count=$OPTARG
	   ;;
	h) echo "-c - gives the script a count of how many times to write lorem ipsum paragraph to data file."
	\?) echo "-$OPTARG: Requires Arguments"
    esac
done

file="lorem_ipsum"
lorem_ipsum="Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

for i in $(seq 1 $count)
do
    echo $lorem_ipsum >> $file
    i=$((i + 1))
done

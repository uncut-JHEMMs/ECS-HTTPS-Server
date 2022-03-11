#!/bin/bash

count=3

while getopts "c:b" opt
do
    case $opt in
	c) count=$OPTARG
	   ;;
	b) big=1
	   ;;
	\?) echo "-$OPTARG: Requires Arguments"
    esac
done

file="lorem_ipsum1"

if [ $big -eq 0 ]
then
    for i in $(seq 1 $count)
    do
	for ii in $(seq 1 $count)
	do
	    cat lorem_ipsum >> $file
	    ii=$((ii + 1))
	done
	i=$((i + 1))
	file="lorem_ipsum$i"
    done
else
    for i in $(seq 1 $count)
    do
	cat lorem_ipsum >> lorem_ipsumbig
    done    
fi

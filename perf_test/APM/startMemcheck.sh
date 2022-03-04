#!/bin/bash

clear

ofile=output/memcheck_output
exec=exec
config=config

while getopts "o:e:c:" opt
do
    case $opt in
	o) ofile=$OPTARG
	   ;;
	e) exec=$OPTARG
	   ;;
	c) config=$OPTARG
	   ;;
	\?) echo "-$OPTARG: Invalid Argument"
	    exit 1
	    ;;
    esac
done

if [ $(clang++ -g -Wall -o $exec ../../src/main.cpp -lhttpserver -ljsoncpp -pthread) ]
then
    echo "failed to compile code"
    exit 1
fi

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$exec $config &> $ofile

rm -r $exec
exit 0

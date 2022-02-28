#!/bin/bash

exec=exec
config=config
ofile=output/massif.out

while getopts "e:c:o:h" opt
do
    case $opt in
	o) ofile=$OPTARG
	   ;;
	e) exec=$OPTARG
	   ;;
	c) config=$OPTARG
	   ;;
	h) echo "-e: executable file name"
	   echo "-c: Server configuration file name"
	   echo "-o: Output file name"
	   exit 1
	   ;;
	?) echo "ERROR: -$OPTARG requires arguments"
	exit 1
	;;
    esac
done

if [ ! -f $config ]
then
    echo "$config: File does not exist"
    exit 1
fi

if [ ! $(clang++ -g -Wall -o $exec ../../src/main.cpp -lhttpserver -ljsoncpp -pthread) ]
then
    valgrind --tool=massif --stacks=yes --massif-out-file=$ofile ./$exec $config
    ms_print $ofile
    
    rm -r $exec
fi

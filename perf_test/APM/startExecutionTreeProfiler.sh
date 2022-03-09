#!/bin/bash

exec=exec
config=config
ofile=output/massif.out

while getopts "e:c:o:bh" opt
do
    case $opt in
	o) ofile=$OPTARG
	   ;;
	e) exec=$OPTARG
	   ;;
	c) config=$OPTARG
	   ;;
	b) bytes_flag=true
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
    
    valgrind --tool=massif --stacks=yes --xtree-memory=full --xtree-memory-file=$ofile --massif-out-file=output/heapProfile.out ./$exec $config 
    
    kcachegrind $ofile
	
    ms_print output/heapProfile.out
    
    rm -r $exec
fi

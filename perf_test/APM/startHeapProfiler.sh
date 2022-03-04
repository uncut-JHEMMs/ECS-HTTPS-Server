#!/bin/bash

exec=exec
config=config
ofile=output/ofile

while getopts "e:c:o:h" opt
do
    case $opt in
	c) config=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	e) exec=$OPTARG
	   ;;
	h) echo "-e: Executable file name"
	   echo "-c: Server configuration file name"
	   echo "-o: Output file name"
	   exit 1
	   ;;
	?) echo "ERROR: -$OPTARG needs an argument"
	exit 1
    esac
done

if [ ! -f $config ]
then
    echo "$config: File does not exist"
    exit 1
fi

if [ ! $(clang++ -o $exec -g -Wall ../../src/main.cpp -lhttpserver -ljsoncpp -pthread) ]
then
    valgrind --tool=massif --massif-out-file=$ofile ./$exec $config
    ms_print $ofile
    rm -r $exec
    exit 0

else
    exit 1
fi


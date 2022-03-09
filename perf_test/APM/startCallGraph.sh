#!/bin/bash

exec=exec
config=config
ofile=output/callgrind.out

while getopts "e:c:o:h" opt
do
    case $opt in
	e) exec=$OPTARG
	   ;;
	c) config=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	h) echo "-e: executable name"
	   echo "-c: Server configuration file name"
	   echo "-o: Output file name"
	   exit 1
	   ;;
	?) echo "ERROR: Invalid argument for -$OPTARG"
	exit 1
	;;
    esac
done

if [ ! -f $config ]
then
    echo "$config: File does not exist"
    exit 1
fi

if [ ! $(clang++ -o $exec -g -Wall ../../src/main.cpp -lhttpserver -ljsoncpp -pthread) ]
then

    valgrind --tool=callgrind --callgrind-out-file=$ofile ./$exec $config

    echo
    echo
    echo
    
    callgrind_annotate $ofile

    kcachegrind $ofile
    
    rm -r $exec
    exit 0

else
    exit 1
fi

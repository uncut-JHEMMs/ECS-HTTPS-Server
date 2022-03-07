#!/bin/bash

exec=exec
config=config
file=output/cachegrind.out

while getopts "f:e:c:h" options
do
    case $options in
	f)
	    file=$OPTARG
	    ;;
	e)
	    exec=$OPTARG
	    ;;
	c)
	    config=$OPTARG
	    ;;
	h) echo "-f: output file option"
	   echo "-e: executable name"
	   echo "-c: server configuration file"
	   exit 1
	   ;;
	*) echo "ERROR: -${OPTARG} requires an argument"
    esac
done

if [ ! $(clang++ -g -Wall -o $exec ../../src/main.cpp -lhttpserver -ljsoncpp -pthread) ]
then
 	
    if [ ! -f $config ]
    then
	echo"$config: Does not exist"
    fi
    
    valgrind --tool=cachegrind --cachegrind-out-file=$file ./$exec $config 

    rm -r $exec
    exit 0
else
    echo "Failed to compile server"
    exit 1
fi
#!/bin/bash

exec=exec
config=config
ofile=output/ofile

while getopts "e:c:o:bh" opt
do
    case $opt in
	c) config=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	b) bytes_flag=true
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
    if [ $bytes_flag == true ]
    then
echo flag
	valgrind --tool=massif --stacks=yes --time-unit=B --massif-out-file=$ofile ./$exec $config

    else
	valgrind --tool=massif --stacks=yes --massif-out-file=$ofile ./$exec $config
    fi
    
    ms_print $ofile
    rm -r $exec
    exit 0

else
    exit 1
fi


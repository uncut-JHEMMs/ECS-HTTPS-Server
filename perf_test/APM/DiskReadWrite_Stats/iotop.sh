#!/bin/bash

exec=exec
iterations=3
while getopts "e:n:" opt
do
    case $opt in
	e) exec=$OPTARG
	   ;;
	n) iterations=$OPTARG
	   ;;
	\?) echo "ERROR: -OPTARG arguments required"
	    exit 1
    esac
done

pid=$(pgrep $exec)

if [ ! $pid ]
then
    echo "Failed to find pid"
    exit 1
fi

sudo iotop -b -p $pid -n $iterations -k | grep -A 1 Total | awk -f awkFile.awk

python3 plot.py tmpfile

rm -r tmpfile

exit 0


#!/bin/bash

exec=exec

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

if [ $iterations > 0 ]
then
    sudo iotop -b -p $pid -n $iterations
    exit 0
else
    sudo iotop -b -p $pid
    exit 0
fi

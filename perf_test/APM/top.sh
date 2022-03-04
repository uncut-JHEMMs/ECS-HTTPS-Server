#!/bin/bash

exec=exec

while getopts "e:" opt
do
    case $opt in
	e) exec=$OPTARG
	   ;;
	\?) echo "-$OPTARG: invalid arguments"
	    exit 1
    esac
done

pid=$(pgrep $exec)
if [ ! $pid ]
then
    echo "Could not find process $exec"
    exit 1
fi

top -p $pid
exit 0

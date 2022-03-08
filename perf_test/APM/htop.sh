#!/bin/bash

#interactively monitor the system's vital resources or server's processes in real time.
exec=exec
while getopts "e:" opt
do
    case $opt in
	e) exec=$OPTARG
	   ;;
	/?) echo "ERROR: -$OPTARG requires arguments"
    esac
done

pid=$(pgrep $exec)

if [ ! $pid ]
then
    echo "Process was not found. Exitting"
    exit 1
fi

htop -p $pid

exit 0

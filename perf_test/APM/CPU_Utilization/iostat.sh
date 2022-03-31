#!/bin/bash

time=1
interval=3

while getopts "c:i:" opt
do
    case $opt in
	c) time=$OPTARG
	   ;;
	i) interval=$OPTARG
	   ;;
	\?) echo "ERROR: -$OPTARG invalid argument"
	    exit 1
    esac
done

iostat $time $interval -c >> tmpfile

#rm -r tmpfile

exit 0

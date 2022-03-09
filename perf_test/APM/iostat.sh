#!/bin/bash

while getopts "c:i:" opt
do
    case $opt in
	c) count=$OPTARG
	   ;;
	i) interval=$OPTARG
	   ;;
	\?) echo "ERROR: -$OPTARG invalid argument"
	    exit 1
    esac
done

iostat $count $interval -t -p ALL
exit 0

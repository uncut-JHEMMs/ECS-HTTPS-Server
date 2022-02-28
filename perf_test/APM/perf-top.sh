#!/bin/bash

while getopts "e:" options
do
    case $options in
	e) pid=$(pgrep $OPTARG)
	   
	   sudo perf top -p $pid
	   exit 0
	   ;;
	?) echo "ERROR: -${OPTARG} requires an argument"
	;;
    esac
done

sudo perf top

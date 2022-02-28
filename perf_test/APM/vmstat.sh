#!/bin/bash

while getopts "c:i:" opt
do
    case $opt in
	c) count=$OPTARG
	   ;;
	i) interval=$OPTARG
	   ;;
	\?) echo "-$OPTARG: Invalid arguments"
	    exit 1
    esac
done

vmstat $interval $count
exit 0

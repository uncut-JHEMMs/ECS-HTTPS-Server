#!/bin/bash

while getopts "c:i:" opt
do
    case $opt in
	c) count=$OPTARG
	   ;;
	i) interval=$OPTARG
	   ;;
	\?) echo "-$OPTARG: Invalid Arguments"
	    exit 1
    esac
done
	
mpstat -P ALL $interval $count
exit 0

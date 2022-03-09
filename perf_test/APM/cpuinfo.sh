#!/bin/bash

#Prints cpu information such as processor type and cores.
ofile=output/cpuinfo.out

while getopts o: opt
do
    case $opt in
	o) ofile=$OPTARG
	   ;;
	\?) echo "-$OPTARG: Requires arguments"
	    exit 1
    esac
done

cat /proc/cpuinfo > $ofile
    

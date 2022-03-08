#!/bin/bash

#if the -f flag is used with an argument then a file name/path can be set. Otherwise, the default one will be used.

clear
file=output/system_config

while getopts "f:" options
do
    case $options in
	f)
	    file=$OPTARG
	    ;;
	*)
	    usage
    esac
done

echo "CPU and cores" > $file
cat /proc/cpuinfo | grep -m 1 "model name" >> $file
cat /proc/cpuinfo | grep -m 1 "cpu cores" >> $file

echo >> $file
echo "RAM and Swap usage memory" >> $file

free -h >> $file

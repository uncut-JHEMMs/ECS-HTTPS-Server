#!/bin/bash

clear
cfile="server/config"

while getopts "i:" opt
do
    case $opt in
	i) $cfile=$OPTARG
	   ;;
	\?) echo "-$opt: Arguments required"
    esac
done

make

./exec $cfile

make clean

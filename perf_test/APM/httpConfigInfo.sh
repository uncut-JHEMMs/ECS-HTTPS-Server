#!/bin/bash

ifile=config
ofile=output/httpConfigInfo.out

while getopts o:i: opt
do
    case $opt in
	o) ofile=$OPTARG
	   ;;
	i) ifile=$OPTARG
	   ;;
	\?) echo "-$OPTARG: Requires arguments"
    esac
done

if [ ! -f $ifile ]
then
    echo "Config file does not exist"
fi

cat $ifile > $ofile

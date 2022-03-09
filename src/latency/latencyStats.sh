#!/bin/bash

format_file=curl-format.txt
count=5
ofile=latency.out

while getopts "c:o:h:" opt
do
    case $opt in
	c) count=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	h) echo "-c: Takes an unsigned int representing number of tests to run"
	   echo "-o: Specifies the output file to use"
	   ;;
	\?) echo "-$OPTARG: Requires arguments"
    esac
done

if [ -f $ofile ]
then
    rm -r $ofile
fi

if [ $count -lt 2 ]
then
    exit 1
fi

for i in $(seq 1 $count)
do
    curl -v -k -w "%{time_total}" -o /dev/null -s "https://localhost:8080/hello" 1>> $ofile 2> /dev/null
    echo >> $ofile
done

jupyter notebook Latency_notebook.ipynb

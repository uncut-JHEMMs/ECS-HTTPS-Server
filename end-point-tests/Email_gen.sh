#!/bin/bash

dest=localhost
port=8080
ofile=emails.xml

while getopts i:d:p:o: opt
do
    case $opt in
	i) ifile=$OPTARG
	   ;;
	d) dest=$OPTARG
	   ;;
	p) port=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	\?) echo "-$opt: Arguments required."
	    ;;
    esac
done

if [ ! -f $ifile ]
then
    echo "file does not exist"
    exit 1
fi

curl -o $ofile -k -d @$ifile "https://$dest:$port/users" &> /dev/null

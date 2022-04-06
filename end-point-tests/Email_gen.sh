#!/bin/bash

dest=localhost
port=8080
ofile=emails.xml

while getopts d:p:o: opt
do
    case $opt in
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

curl -XGET -o $ofile -k "https://$dest:$port/users" &> /dev/null

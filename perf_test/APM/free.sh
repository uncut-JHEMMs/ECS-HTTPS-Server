#!/bin/bash

#need to run with -s option giving it a time interval.
while getopts "s:" opt
do
    case $opt in
	s)
	    #Doesn't return
	    free -h -t -s $OPTARG
	    ;;
	\?)
	    echo "ERROR: -$OPTARG requires arguments"
	    exit 1
	    ;;
    esac
done

free -h -t
exit 0

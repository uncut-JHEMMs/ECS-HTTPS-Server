#!/bin/bash

#need to run with -s option giving it a time interval.
while getopts "s:" opt
do
    case $opt in
	s)
	    #Doesn't return
	    free -h -t -w -s $OPTARG
	    ;;
	\?)
	    echo "ERROR: -$OPTARG requires arguments"
	    exit 1
	    ;;
    esac
done

free -b -t -w -s 1 -c 3 | grep Swap: | cut -d ":" -f 2 >> tmpfile

python3 SwapMem.py tmpfile

rm -r tmpfile

exit 0

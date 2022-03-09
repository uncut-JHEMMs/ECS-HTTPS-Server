#!/bin/bash

cd ..

exec=exec

if [ ! $(clang++ -o $exec -g -Wall ../../src/main.cpp -lhttpserver -ljsoncpp -pthread) ]
then

    ./$exec config &

    ./free.sh > /dev/null
    
    if [ ! $? ]
    then
	echo "FREE TEST FAILED"
	exit 1
    fi
    
    ./htop.sh -e $exec 
    
    if [ ! $? ]
    then
	echo "HTOP FAILED"
	exit 1
    fi
    
    ./iftop.sh > /dev/null
    
    if [ ! $? ]
    then
	echo "IFTOP TEST FAILED"
	exit 1
    fi
    
    ./iostat.sh > /dev/null
    
    if [ ! $? ]
    then
	echo "IOSTAT TEST FAILED"
	exit 1
    fi
    
    ./iotop.sh -n 1 > /dev/null
    ./iotop.sh -e $exec -n 1 > /dev/null
    
    if [ ! $? ] && [ ! $? ]
    then
	echo "IOTOP TEST FAILED"
	exit 1
    fi
    
    ./mpstat.sh > /dev/null
    
    if [ ! $? ]
    then
	echo "MPSTAT TEST FAILED"
	exit 1
    fi
    
    ./top.sh > /dev/null
    
    if [ ! $? ]
    then
	echo "TOP TEST FAILED"
	exit 1
    fi
    
    ./vmstat.sh > /dev/null
    
    if [ ! $? ]
    then
	echo "VMSTAT TEST FAILED"
	exit 1
    fi
    
    echo "ALL TESTS PASSED"

    pid=$(pgrep $exec)
    sudo kill $pid
    
else
    echo "failed compilation"
    exit 1
fi

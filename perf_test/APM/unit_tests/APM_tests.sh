#!/bin/bash

../free.sh > /dev/null

if [ ! $? ]
then
    echo "FREE TEST FAILED"
    exit 1
fi

../htop.sh 

if [ ! $? ]
then
    echo "HTOP TEST FAILED"
    exit 1
fi

../htop.sh -e exec 

if [ ! $? ]
then
    echo "HTOP FLAGS FAILED"
    exit 1
fi

../iftop.sh > /dev/null

if [ ! $? ]
then
    echo "IFTOP TEST FAILED"
    exit 1
fi

../iostat.sh > /dev/null

if [ ! $? ]
then
    echo "IOSTAT TEST FAILED"
    exit 1
fi

../iotop.sh -n 1 > /dev/null
../iotop.sh -e exec -n 1 > /dev/null

if [ ! $? ] && [ ! $? ]
then
    echo "IOTOP TEST FAILED"
    exit 1
fi

../mpstat.sh > /dev/null

if [ ! $? ]
then
    echo "MPSTAT TEST FAILED"
    exit 1
fi

../top.sh > /dev/null

if [ ! $? ]
then
    echo "TOP TEST FAILED"
    exit 1
fi

../vmstat.sh > /dev/null

if [ ! ../vmstat.sh ]
then
    echo "VMSTAT TEST FAILED"
    exit 1
fi

cd ..; ./startCacheBranchProfiler.sh -c config > /dev/null

if [ ! $? ]
then
    echo "CACHE BRANCH PROFILER FAILED"
    exit 1
fi

./startCallGraph.sh -c config > /dev/null

if [ ! $? ]
then
    echo "CALL GRAPH FAILED"
    exit 1
fi

./startExecutionTreeProfiler.sh -c config > /dev/null

if [ ! $? ]
then
    echo "EXECUTION TREE PROFILER FAILED"
    exit 1
fi

./startHeapProfiler.sh > /dev/null

if [ ! $? ]
then
    echo "HEAP PROFILER FAILED"
    exit 1
fi

./startMemcheck.sh > /dev/null

if [ ! $? ]
then
    echo "MEMCHECK FAILED"
    exit 1
fi

echo "ALL TESTS PASSED"

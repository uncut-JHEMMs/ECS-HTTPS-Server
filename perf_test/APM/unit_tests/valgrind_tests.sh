#!/bin/bash

cd ..; ./startCacheBranchProfiler.sh -c config -o cacheBranchProfiler.out &> /dev/null

if [ ! $? ]
then
    echo "CACHE BRANCH PROFILER FAILED"
    exit 1
fi

./startCallGraph.sh -c config -o callgraph.out &> /dev/null

if [ ! $? ]
then
    echo "CALL GRAPH FAILED"
    exit 1
fi

./startExecutionTreeProfiler.sh -c config -o executionTree.out &> /dev/null

if [ ! $? ]
then
    echo "EXECUTION TREE PROFILER FAILED"
    exit 1
fi

./startHeapProfiler.sh -o heapProfiler.out &> /dev/null

if [ ! $? ]
then
    echo "HEAP PROFILER FAILED"
    exit 1
fi

#!/bin/bash

infile=card_transaction.v1.csv
ofile=generated_data
seekPoints=seekPoints

while getopts i:ho:t:s: opt
do
    case $opt in
	i) infile=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	t) test=$OPTARG
	   ;;
	s) seekPoints=$OPTARG
	   ;;
	h) echo "-i: Takes arguments of a file name to prcess."
	   echo "-o: Takes arguments of an output file name to process."
	   echo "-t: Takes as the argument the test to run. Current tests: seekPoints."
	   echo "-h: Returns this."
    esac
done

clear

if [ "$test" == "seekPoints" ]
then
    clang++ -Ofast -std=c++17 testSeekPoints.cpp 
    ./a.out $seekPoints $ofile
    rm -r a.out
else
    clang++ -Ofast -std=c++17 generate_name_emails.cpp 
    
    time ./a.out $infile $ofile $seekPoints
    
    rm -r a.out 

fi

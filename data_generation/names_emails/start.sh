#!/bin/bash

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
	   echo "-o: Takes arguments of an output file name."
	   echo "-t: Takes as the argument the test to run. Current tests: seekPoints."
	   echo "-s: This takes a seekpoints file argument for testing."
	   echo "-h: Returns this."
	   exit 0
    esac
done

clear

if [ "$test" == "seekPoints" ]
then
    if [ ! -f "$seekPoints" ]
    then
	echo "$seekPoints: Does not exist."
	exit 1
    fi
    
    if [ "$seekPoints" == "" ]
    then
	echo "seekPoints file not provided."
	exit 1
    fi
    
    clang++ -Ofast -std=c++17 testSeekPoints.cpp 
    ./a.out $seekPoints $ofile
    rm -r a.out
else
    clang++ -Ofast -std=c++17 generate_name_emails.cpp 
    
    count=$(cat $infile | awk -F ',' '
BEGIN{check}
{
	if(NR != 1) {     
	      if($1 == 0 && check != 1){
	      	    check = 1
	      	    ids[0] = 0
	      	    print $1 > "tmpfile"
	      }
	      else if($1 != 0){
	      	   if($1 > 0 && ids[$1] == 0){
	     	   	 ids[$1] = $1
	     	   	 print $1 > "tmpfile"	    
       	     	   }
	      }
        }
}')

    ./a.out tmpfile

    rm -r a.out tmpfile

fi

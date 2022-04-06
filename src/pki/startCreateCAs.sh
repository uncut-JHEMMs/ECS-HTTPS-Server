#!/bin/bash

size=0
infile=""

while getopts "ht:i:" opt
do
    case $opt in
        
	t) test=1
	   files+=($OPTARG)
	   size=$((size + 1))
	   ;;

	i) infile=$OPTARG
	   ;;
	
	h) echo "-t: Tells script to enable test mode. This needs to be followed by three files. Each being a certificate. This will test the Root CA and the Server and Document CA's."
	   echo "-h: Tells the script to display this information."
	   ;;   

	\?) echo "-$opt: requires arguments."
    esac
done

if [ "$test" == "1" ] && [ "$size" == "3" ]
then
    openssl x509 -noout -text -in ${files[0]}
    echo
    echo "Test if ${files[1]} was signed by root ca."
    openssl verify -CAfile ${files[0]} ${files[1]}
    echo
    echo "Test if ${files[2]} was signed by root ca."
    openssl verify -CAfile ${files[0]} ${files[2]}
    
    exit $?
    
elif [ "$test" == "1" ]
then
    echo "Improper number of files provided"
    exit 1
fi	   

if [ -f $infile ] && [ "$infile" != "" ]
then
    clang++ -std=c++17 createCAs.cpp $(pkg-config --cflags --libs openssl) -ljsoncpp
    
    ./a.out $infile
    
    rm -r a.out *.o

    exit 0

else
    echo "File does not exist or flag not used."
    exit 1
fi

exit 1

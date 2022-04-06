#!/bin/bash

clear

config=server/config

while getopts i: opt
do
    case $opt in
	i) config=$OPTARG
	   ;;
	\?) echo "-$opt: Requires Arguments."
    esac
done

if [ ! -f "$config" ]
then
    echo "$config file does not exist"
    exit 1
fi

make

./exec $config

make clean
rm -r signedUserData.xml

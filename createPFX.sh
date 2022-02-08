#!/bin/bash

clear

cert=$1
key=$2
pfx=$3

if [ $1 == test ]
then
    echo "Test start..."
    openssl pkcs12 -info -in $2
else
    
    openssl pkcs12 -export -out $pfx -inkey $key -in $cert

fi

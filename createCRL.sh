#!/bin/bash

clear

crl=$1

if [ $1 == test ]
then
    echo "Test start..."
    openssl crl -in $2 -noout -text
    
else
    
#Generate CRL
openssl ca -config openssl-ca.cnf -gencrl -out $crl

fi

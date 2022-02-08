#!/bin/bash

clear

key=$1
cert=$2

if [ $1 == test ] || [ $1 == Test ]
then
    #test if certificate was generated
    echo "Verify root CA certificate"
    openssl x509 -noout -text -in $cert

    
#Generate CA.
else
    
    #Generate a private key
    openssl genrsa -aes256 -out $key 4096
    chmod 400 $key

    #Generate the CA
    openssl req -new -x509 -config openssl-ca.cnf -days 365000 -key $key -out $cert
    chmod 444 $cert

fi

#!/bin/bash

if [ "$1" == "test" ] || [ "$1" == "Test" ]
then
#test if certificate was generated     
    openssl x509 -noout -text -in root/ca/certs/ca.cert.pem

elif [ $1 == clean ]
then
    mv root/ca/openssl.cnf .
    rm -r root/**
    rmdir root
    
else
    mkdir root
    mkdir root/ca
    mkdir root/ca/private
    mkdir root/ca/certs
    mkdir root/ca/newcerts
    mkdir root/ca/crl
    touch root/ca/index.txt
    mv openssl.cnf root/ca/
    
    chmod 700 root/ca/private
    
    openssl genrsa -aes256 -out root/ca/private/ca.key.pem 4096
    chmod 400 root/ca/private/ca.key.pem
    
    openssl req -new -x509 -config root/ca/openssl.cnf -nodes -days 365000  -key root/ca/private/ca.key.pem -out root/ca/certs/ca.cert.pem
    chmod 444 root/ca/certs/ca.cert.pem
fi

#!/bin/bash

clear

if [ "$1" == "test" ] || [ "$1" == "Test" ]
then
    #test if certificate was generated
    echo "Verify root CA certificate"
    openssl x509 -noout -text -in root/ca/certs/root.ca.cert.pem
    echo "Verify server csr was signed by root CA"
    openssl verify -CAfile root/ca/certs/root.ca.cert.pem server/ca/certs/server.ca.cert.pem
    
elif [ $1 == clean ]
then
    rm -r root/**
    rmdir root
    rm -r server/**
    rmdir server
    
#Generate local root CA. Then sign a server certificate.
else
    mkdir root
    mkdir root/ca

    cd root/ca/
    
    mkdir private certs newcerts crl
    touch root/ca/index.txt
    touch root/ca/serial
    
    chmod 700 root/ca/private
    touch index.txt
    echo '01' > serial
    
    #Generate a private key
    openssl genrsa -aes256 -out private/root.ca.key.pem 4096
    chmod 400 private/root.ca.key.pem

    #Generate the CA
    openssl req -new -x509 -config ../../openssl-ca.cnf -days 365000 -key private/root.ca.key.pem -out certs/root.ca.cert.pem
    chmod 444 certs/root.ca.cert.pem

    cd ../../
    mkdir server
    mkdir server/ca
    cd server/ca/

    mkdir private certs
    touch index.txt
    echo "01" > serial
    
    chmod 700 private

    #Generate server private key
    openssl genrsa -aes256 -out private/server.ca.key.pem 4096
    chmod 400 private/server.ca.key.pem

    openssl req -new -key private/server.ca.key.pem -out server.csr
    
    echo
    echo
    echo "Generate certificate from csr"
    cd ../../

    #Generate a certificate for the server from .csr
    openssl ca -config openssl-ca.cnf -out server/ca/certs/server.ca.cert.pem -infiles server/ca/server.csr
fi

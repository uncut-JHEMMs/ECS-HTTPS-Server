#!/bin/bash

if [ $1 -eq test ]
then
    echo "Verify server csr was signed by root CA"                           
    openssl verify -CAfile root/ca/certs/root.ca.cert.pem server/ca/certs/server.ca.cert.pem

else
    
    mkdir server
    mkdir server/ca    

    cd server/ca/

    mkdir private certs clr

    touch index.txt
    echo "01" > serial
    chmod 700 private

    #Generate server private key                                            
    openssl genrsa -aes256 -out private/server.ca.key.pem 4096
    chmod 400 private/server.ca.key.pem

    #Generate server csr
    openssl req -new -key private/server.ca.key.pem -out server.csr
    
    echo
    echo
    echo "Generate certificate from csr"
    
    cd ../../
    
    #Generate a certificate for the server from .csr
    
    openssl ca -config openssl-ca.cnf -out server/ca/certs/server.ca.cert.pem -infiles server/ca/server.csr
    
fi

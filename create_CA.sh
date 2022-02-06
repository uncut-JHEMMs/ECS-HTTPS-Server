#!/bin/bash

clear

if [ $1 -eq test ] || [ $1 -eq Test ]
then
    #test if certificate was generated
    echo "Verify root CA certificate"
    openssl x509 -noout -text -in root/ca/certs/root.ca.cert.pem
    
elif [ $1 == clean ]
then
    rm -r root/**
    rmdir root
    
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
    echo 1000 > crlnumber
    
    #Generate a private key
    openssl genrsa -aes256 -out private/root.ca.key.pem 4096
    chmod 400 private/root.ca.key.pem

    #Generate the CA
    openssl req -new -x509 -config ../../openssl-ca.cnf -days 365000 -key private/root.ca.key.pem -out certs/root.ca.cert.pem
    chmod 444 certs/root.ca.cert.pem

fi

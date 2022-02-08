#!/bin/bash

csr=$1
signingcert=$2
signedcert=$3

if [ $1 == test ]
then
    echo "Verify server csr was signed by root CA"                           
    openssl verify -CAfile $signingcert $signedcert

else
       
    #Generate a certificate for the server from .csr
    openssl ca -config openssl-ca.cnf -out $signingcert -infiles $csr
    
fi

#!/bin/bash

clear

if [ $1 == test ]
then
    echo "Test start..."
    openssl crl -in root/ca/crl/root.ca.crl.pem -noout -text
    
else
    
#Generate CRL
openssl ca -config openssl-ca.cnf -gencrl -out root/ca/crl/root.ca.crl.pem

#Add server to revokation list
openssl ca -config openssl-ca.cnf -revoke server/ca/certs/server.ca.cert.pem

fi

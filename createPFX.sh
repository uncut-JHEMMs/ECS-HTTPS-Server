#!/bin/bash

clear

if [ $1 == test ]
then
    echo "Test start..."
    openssl pkcs12 -info -in server/ca/pfx/server.ca.bundle.pfx
else

    mkdir server/ca/pfx
    
    openssl pkcs12 -export -out server/ca/pfx/server.ca.bundle.pfx -inkey server/ca/private/server.ca.key.pem -in server/ca/certs/server.ca.cert.pem

fi

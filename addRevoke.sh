#!/bin/bash

if [ $1 == test ]
then
    db=$2

    echo "The previous revoke should have an R before the entry."
    cat $db
    
else
    
cert=$1
#Add certificate to revokation list
openssl ca -config openssl-ca.cnf -revoke $cert

fi

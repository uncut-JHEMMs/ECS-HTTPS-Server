#!/bin/bash

if [ $1 == test ]
then
    db=$2

    echo "The previous revoke should have an R before the entry."
    cat $db
    
else
    
    conf=$1
    cert=$2
    #Add certificate to revokation list
    openssl ca -config $conf -revoke $cert

fi

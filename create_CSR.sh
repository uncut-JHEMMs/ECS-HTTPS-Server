#!/bin/bash

key=$1
csr=$2

#Generate server private key                                              
openssl genrsa -aes256 -out $key 4096

chmod 400 $key

#Generate server csr
openssl req -new -key $key -out $csr

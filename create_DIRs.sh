#!/bin/bash

mkdir root root/ca

cd root/ca/

mkdir crl private certs newcerts
echo "01" > serial
echo 1000 > crlnumber
touch index.txt

chmod 700 private

cd ../../

mkdir server server/ca/

cd server/ca/

mkdir crl private certs newcerts pfx

echo "01" > serial
echo 1000 > crlnumber

chmod 700 private

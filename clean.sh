#!/bin/bash

rm -r root/**
rmdir root

mv server/ca/openssl-sca.cnf .
rm -r server/**
rmdir server

mv document/ca/openssl-dca.cnf .
rm -r document/**
rmdir document

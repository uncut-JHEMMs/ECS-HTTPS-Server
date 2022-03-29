#!/bin/bash

clear

clang++ -O3 -std=c++17 main.cpp 

./a.out $1

rm -r a.out 

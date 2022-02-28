#!/bin/bash

clear

sudo iftop -i lo -p -n -o 127.0.0.1
exit 0

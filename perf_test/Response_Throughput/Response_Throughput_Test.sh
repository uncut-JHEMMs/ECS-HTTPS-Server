#!/bin/bash

# Uses a file full of data and calculates the max throughput
function max(){

    local max=0
    while read -r line
    do
	line=${line%.*}
	if [ $line -gt $max ]
	then
	    max=$line
	fi
    done < tmpfile
    echo $max
}

dest=localhost
port=8080
tcount=1
variable=""
size=0

while getopts "hv:d:p:i:n:" opt
do
    case $opt in
	v) variable=$OPTARG
	   ;;
	i) data+=($OPTARG)
	   size=$((size + 1))
	   ;;
	n) tcount=$OPTARG
	   ;;
	d) dest=$OPTARG
 	   ;;
	p) port=$OPTARG
	   ;;
	h) echo "-v: variable payload on (0) variable payload off (1)"
	   echo "-i: data file to send"
	   echo "-n: Thread count"
	   echo "-d: The destination to send the data"
	   echo "-p: The port to send the data to"
	   ;;
	\?) echo "-$opt: Arguments Required"
    esac
done

if [ "$data[0]" == "" ] || [ "$variable" == "" ]
then
    echo "data file or variable payload not specified. Exitting."
    exit 1
fi

tmpfile=tmpfile

#sends variable or non-variable paylods asynchronously to the server and puts the upload times in a flat file for parsing.
if [ $variable -eq 1 ]
then
    for i in $(seq 0 $((tcount - 1)))
    do
	curl -k -s -o /dev/null -d @${data[0]} -w "%{speed_upload}\n" https://$dest:$port/hello &>> $tmpfile &
    done

    wait

    max_val=$(max)
else
    #variable payload selected.
    for i in $(seq 0 $((tcount - 1)))
    do
	curl -k -s -o /dev/null -d @${data[$(expr $i % $size)]} -w "%{speed_upload}\n" https://$dest:$port/hello &>> $tmpfile &
    done

    wait

    max_val=$(max)
fi

#converts float value of bits per second to MB per second. Bash can't seem to operate on floats.
python3 conversion.py $max_val

rm -r tmpfile

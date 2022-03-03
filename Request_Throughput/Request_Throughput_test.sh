#!/bin/bash

#seq 1 100 | xargs -n 2 -P 50 -I {} curl -o /dev/null -s -w "%{http_code}\n" -k "https://localhost:8080/hello"

success=0
fail=1
avg_count=1
port=8080
dest=localhost
ofile=request_throughput.csv

while getopts "p:d:c:o:" opt
do
    case $opt in
	p) port=$OPTARG
	   ;;
	d) dest=$OPTARG
	   ;;
	c) avg_count=$OPTARG
	   ;;
	o) ofile=$OPTARG
	   ;;
	\?) echo "-$OPTARG: requires arguments"
	    exit 1
    esac
done
echo "success,fail" > $ofile

start=$SECONDS

tmpfile=status_codes
current_ratio=-1
next_ratio=0
succ_sum=0
fail_sum=0
total=1

for ii in $(seq 1 $avg_count)
do
    while [ $current_ratio -lt $next_ratio ]
    do
	current_ratio=$next_ratio
	
	for i in $(seq 1 $total)
	do
	    curl -o /dev/null -s -w "%{http_code}\n" -k "https://$dest:$port/hello" &>> $tmpfile &
            
	done
	
	wait
	while read -r line
	do
	    if [ "$line" == 200 ]
	    then
		success=$((success + 1))
	    else
		fail=$((fail + 1))
	    fi
	done < $tmpfile

	echo "success to fail"
	echo $success : $fail
	
	next_ratio=$((success / fail))
	total=$((total + 1))
	
	echo "$success,$fail" >> $ofile
	
	succ_sum=$((succ_sum + success))
	fail_sum=$((fail_sum + fail))
	
	success=0
	fail=1
	
	duration=$(( SECONDS - start ))
	
	rm -r $tmpfile
	
	echo "Duration: $duration"
    done
    echo $ii
done

duration=$(( SECONDS - start ))

average_succ=$((succ_sum / avg_count))
average_fail=$((fail_sum / avg_count))

average_max_ratio=$((average_succ / average_fail))

echo "Duration: $duration"
echo "Average max ratio $average_max_ratio"

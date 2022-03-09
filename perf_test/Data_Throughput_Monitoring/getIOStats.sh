#!/bin/bash

ofile=fio_out
block_size=4k
size=4g
run_time=60

echo "Block Size: $block_size"
echo "Size: $size" 

fio --name=random-write --ioengine=posixaio --rw=randwrite --bs=$block_size --numjobs=1 --size=$size --iodepth=1 --runtime=$run_time --time_based --end_fsync=1

rm -r random.write*

block_size=16k
size=16g

echo
echo
echo "Block Size: $block_size"
echo "Size: $size"

fio --name=random-write --ioengine=posixaio --rw=randwrite --bs=$block_size --numjobs=1 --size=$size --iodepth=1 --runtime=$run_time --time_based --end_fsync=1

rm -r random.write*

block_size=64k
size=64g

echo
echo
echo "Block Size: $block_size"
echo "Size: $size"

fio --name=random-write --ioengine=posixaio --rw=randwrite --bs=$block_size --numjobs=1 --size=$size --iodepth=1 --runtime=$run_time --time_based --end_fsync=1

rm -r random.write*

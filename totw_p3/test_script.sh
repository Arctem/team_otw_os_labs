#!/bin/bash

for i in `seq 0 10`
do
    
    for j in `seq 0 10`
    do
	echo "$i producers and $j consumers in FIFO"
	./p3 $i $j 0
	echo "$i producers and $j consumers in FILO"
	./p3 $i $j 1
    done
    
done

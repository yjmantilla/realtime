#!/bin/bash

FILE1="$1"
NLOOPS=$2
FILE2="$3"

#echo $FILE1;
#echo $FILE2;
#echo $NLOOPS;

rm $FILE2.txt;

#for i in {$(1..100)}; 
for (( i=1; i<=$NLOOPS; i++ ))
do
	./$FILE1 >> $FILE2.txt;
done

gedit $FILE2.txt;
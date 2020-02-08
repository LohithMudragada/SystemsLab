#!/bin/sh


echo "Enter Word : "
read word

count=$(grep -c "$word :" MyDictionary.txt)

if [ $count -ne 0 ] 
then 
	grep "$word :" MyDictionary.txt
else
	echo "Word Isn't Exist"
fi

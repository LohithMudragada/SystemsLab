#!/bin/sh


echo "Enter Word : "
read word

count=$(grep -c "$word :" MyDictionary.txt)

if [ $count -ne 0 ] 
then 
	echo "Word Already Exists!!"
else
	echo "Enter Meaning : "
	read Meaning
	echo "$word : $Meaning">>MyDictionary.txt
	echo "Word Inserted"
	sort -o MyDictionary.txt MyDictionary.txt
	cat MyDictionary.txt
fi

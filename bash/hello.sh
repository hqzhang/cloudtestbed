#!/bin/bash
#debug
set -x
echo search key in file
find . -type f | xargs grep -sinw --color "key"

echo search file
find -name "file"

#replacement
echo "192.168.56.101"|sed 's/101/245/'
msg=`echo '{"reportType" : ""}' | jq -c --arg reportType "mysetval" '.["reportType"]=$reportType ' `
echo $msg
#exit
#get a value
echo "192.168.56.101"|cut -d'.' -f 4
echo "ab cd ef gh"|awk '{print $3}'
echo '{"key":"value"}' |jq -r ".key"
#get a value whole line
echo "192.168.56.101"|grep "56"

#function
function foo {
   echo enter foo
}
foo  
#for loop
for i in $( ls ); do
            echo item: $i
        done
for i in `seq 1 3`;
    do
       echo $i
    done    
#if condition
var="foo"
if [ "$var" = "foo" ]; then
    echo expression evaluated as true
fi
#input
echo Please, enter your name
#read NAME
echo "Hi $NAME!" 

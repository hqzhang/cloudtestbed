#!/bin/bash
#debug
set -x
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
                read NAME
                echo "Hi $NAME!" 

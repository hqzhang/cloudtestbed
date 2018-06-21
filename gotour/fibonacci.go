package main

import "fmt"

// fibonacci is a function that returns
// a function that returns an int.
func fibonacci() func() int {
   a := 1
   b := 0
   tmp:=0
   return func() int {
          //a, b = b, a+b
		  tmp = b  
          b = a + b  
          a = tmp 
        return a
   }
}

func main() {
	f := fibonacci()
	for i := 0; i < 10; i++ {
		fmt.Println(f())
	}
}


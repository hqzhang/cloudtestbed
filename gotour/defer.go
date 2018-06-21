package main

import "fmt"


func main() {
     defer fmt.Println("first")

     for i:=0; i<3 ; i++ {
         defer fmt.Println(i)
     }

}

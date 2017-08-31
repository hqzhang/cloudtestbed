package main

import "fmt"

// Here's a basic interface for geometric shapes.
type Shape interface {
    area() float64
}

type rect struct {
    width, height float64
}
type circle struct {
    radius float64
}

func (r rect) area() float64 {
    return r.width * r.height
}

func measure(g Shape) {
    fmt.Println(g)
    fmt.Println(g.area())
}

func main() {
    //show interface
    r := rect{width: 3, height: 4}
    measure(r)
    //show array vs slice
    var balance [10] int
    for i,num := range balance {
    fmt.Printf("number %d, %d\n", i,num)
   }  
    var numbers []int /* a slice of unspecified size */
    numbers = make([]int,5,5) /* a slice of length 5 and capacity 5*/
    numbers=append(numbers, 9)
    for i,num := range numbers {
    fmt.Printf("number %d, %d\n", i,num)
   }
    
}


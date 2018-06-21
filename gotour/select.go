package main

import "fmt"

func fibonacci(c, quit chan int) {
	x, y := 0, 1
	for {
		select {
		case c <- x:
		    fmt.Println("mainfib:",x)
			x, y = y, x+y
		case <-quit:
			fmt.Println("mainfib:",quit)
			return
		}
	}
}

func main() {
	c := make(chan int)
	quit := make(chan int)
	go func() {
		for i := 0; i < 5; i++ {
			fmt.Println("send:",<-c)
		}
		quit <- 0
	}()
	fmt.Println("call fib()")
	fibonacci(c, quit)
}


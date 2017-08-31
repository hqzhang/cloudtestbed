package main
import "fmt"
import "sync"
import "time"

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
func f(from string) {
    for i := 0; i < 3; i++ {
        fmt.Println(from, ":", i)
    }
}

func main() {
/*    //show interface
    //goroutines, channels, packages, locking primitives, lock avoidance
    f("direct")

    // To invoke this function in a goroutine, use
    // `go f(s)`. This new goroutine will execute
    // concurrently with the calling one.
    go f("goroutine")

    // function call.
    go func(msg string) {
        fmt.Println(msg)
    }("going")

    // before the program exits.
    var input string
    fmt.Scanln(&input)
    fmt.Println("done")
*/
    // show channal
    // make chan
    messages := make(chan string)

    // send a message by <---
    go func() { 
       fmt.Println("send:"+"ping")
       messages <- "ping" }()
    go func() {
                time.Sleep(1*time.Second) 
                fmt.Println("receive1:")
                mymsg:= <-messages 
                fmt.Println("receive:"+mymsg)
                       }()
    // receive a message by <---
    msg := <-messages

    fmt.Println(msg)
    //show lock
    var mylock sync.Mutex
    fmt.Println("lock:", mylock)   
}


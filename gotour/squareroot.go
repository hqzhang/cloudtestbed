package main

import (
    "fmt"
    "math"
)

func Sqrt(x float64) float64 {
    t := 0.
    z := 1.
    for {
        z -= (z*z-x)/(2*z)
        //t = z
        if math.Abs(t-z) < 1e-8 {
            break
        }
        t=z
    }
    return z
}

func main() {
    i := 169.
    fmt.Println(Sqrt(i))
    fmt.Println(Sqrt(i) == math.Sqrt(i))
}

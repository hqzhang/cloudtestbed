package main

import "golang.org/x/tour/pic"
import "fmt"
func Pic(dx, dy int) [][]uint8 {
 fmt.Println(dx,dy)
 pic := make([][]uint8, dy)
  for i := range pic {
    pic[i] = make([]uint8, dx)
    for j := range pic[i] {
      pic[i][j] = uint8(i * j)
    }
  }
  return pic
}

func main() {
	pic.Show(Pic)
}

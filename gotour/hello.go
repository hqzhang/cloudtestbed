package main

import "fmt"

func reverse(s string) string {
        tmps:=[]int32(s)
	chars := []rune(s)
	fmt.Println(chars)
	for i:= 0; i < len(chars)/2; i++ {
            j:=len(chars)-1-i;
	    tmp:=chars[i]
		chars[i] = chars[j]
		chars[j]=tmp
	}
	return string(chars)
}
func main() {
	fmt.Println(reverse("Hello, 世界"))
}

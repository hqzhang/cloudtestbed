package main

import (
	"fmt"
	"sync"
)
var wg sync.WaitGroup
var mux sync.Mutex
//var cacheurl map[string]bool
var cacheurl map[string]bool = make(map[string]bool)
type Fetcher interface {
	// Fetch returns the body of URL and
	// a slice of URLs found on that page.
	Fetch(url string) (body string, urls []string, err error)
	IsProcessed(url string) bool
}
func (f fakeFetcher) IsProcessed(url string) bool {
        mux.Lock()
        defer mux.Unlock()
        if !cacheurl[url] {
                cacheurl[url] = true
                return false
        }
        return true
}

// Crawl uses fetcher to recursively crawl
// pages starting with url, to a maximum of depth.
func Crawl(url string, depth int, fetcher Fetcher, ch chan string) {
    defer close(ch)
	if fetcher.IsProcessed(url) { //check no duplication
              return
      }

	//check bottom
    if depth <= 0 {
        return
    }

    body, urls, err := fetcher.Fetch(url)
    if err != nil {
        ch <- fmt.Sprintln(err)
        return
    }

    ch <- fmt.Sprintf("found: %s %q", url, body)
	result := make([]chan string, len(urls)) //create array
	//fmt.Println("len:",len(urls) )
	//var result [100] chan string
	//don t use make like allocation
	//fmt.Printf("resutl1:%v\n",result)
    for i, u := range urls {
	    
	    result[i] =make(chan string)// a      //create one elecment
		
        go Crawl(u, depth-1, fetcher, result[i])
    }
	for i := range result {
        for s := range result[i] {
            ch <- s
        }
    }

    return
}
   
func main() {
    ch := make(chan string, 100)
    go Crawl("https://golang.org/", 3, fetcher, ch)

    for i := range ch {
        fmt.Println(i)
    }
}

// fakeFetcher is Fetcher that returns canned results.
type fakeFetcher map[string]*fakeResult

type fakeResult struct {
	body string
	urls []string
}

func (f fakeFetcher) Fetch(url string) (string, []string, error) {
	if res, ok := f[url]; ok {
		return res.body, res.urls, nil
	}
	return "", nil, fmt.Errorf("not found: %s", url)
}

// fetcher is a populated fakeFetcher.
var fetcher = fakeFetcher{
	"https://golang.org/": &fakeResult{
		"The Go Programming Language",
		[]string{
			"https://golang.org/pkg/",
			"https://golang.org/cmd/",
		},
	},
	"https://golang.org/pkg/": &fakeResult{
		"Packages",
		[]string{
			"https://golang.org/",
			"https://golang.org/cmd/",
			"https://golang.org/pkg/fmt/",
			"https://golang.org/pkg/os/",
		},
	},
	"https://golang.org/pkg/fmt/": &fakeResult{
		"Package fmt",
		[]string{
			"https://golang.org/",
			"https://golang.org/pkg/",
		},
	},
	"https://golang.org/pkg/os/": &fakeResult{
		"Package os",
		[]string{
			"https://golang.org/",
			"https://golang.org/pkg/",
		},
	},
}


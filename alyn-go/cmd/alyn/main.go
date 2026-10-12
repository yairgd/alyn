package main

import (
	"fmt"
	"os"

	"github.com/yairgd/alyn-go/internal/app"
)

func main() {
	a, err := app.New()
	if err != nil {
		fmt.Fprintf(os.Stderr, "alyn: %v\n", err)
		os.Exit(1)
	}
	defer a.Close()
	a.Run()
}

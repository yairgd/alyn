package sim

import (
	"strings"
	"sync"
	"testing"
	"time"
)

// One C world per process, so a single test drives console, game and keys.
func TestSim(t *testing.T) {
	var mu sync.Mutex
	var out strings.Builder
	var frame Frame
	var prompts int
	post := func(v any) {
		mu.Lock()
		defer mu.Unlock()
		switch m := v.(type) {
		case Output:
			out.WriteString(m.Text)
		case Prompt:
			prompts++
		case Frame:
			frame = m
		}
	}
	Init(post)
	go RunConsole()

	wait := func(what string, ok func() bool) {
		t.Helper()
		for i := 0; i < 100; i++ {
			mu.Lock()
			good := ok()
			mu.Unlock()
			if good {
				return
			}
			time.Sleep(20 * time.Millisecond)
		}
		t.Fatalf("timeout: %s (out=%q)", what, out.String())
	}

	wait("prompt", func() bool { return prompts > 0 })
	SendLine("print(1+1)")
	wait("print output", func() bool { return strings.Contains(out.String(), "2\n") })

	if g := Games(); len(g) == 0 {
		t.Fatal("no games")
	}
	SendLine("print(game.keys())")
	Key(1, false)
	time.Sleep(50 * time.Millisecond)
	SendLine("print(game.keys())")
	wait("key bit", func() bool { return strings.Contains(out.String(), "\n2\n") })

	if !Load("game.clean()\nfor x=0,63 do game.plot(x,4,0xff0000) end\nwhile true do game.delay(100000) end") {
		t.Fatal("load")
	}
	wait("red pixel", func() bool { return len(frame.RGB) > 0 && frame.RGB[(4*MatrixW)*3] != 0 })

	Stop()
	wait("stopped", func() bool { return !Running() })

	Connect(2, false)
	wait("disconnect", func() bool { return !frame.Stations[2].Connected })

	SendLine("x = ") // incomplete: continuation prompt
	wait("continuation prompt", func() bool { return prompts > 4 })
	ConsoleEOF()
}

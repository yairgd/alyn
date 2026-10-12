package ui

import (
	"testing"
	"time"

	"github.com/yairgd/termforge"
)

type key struct {
	id   int
	long bool
}

// The pad acts on release like the Qt ArrawKey: < 500 ms press, longer is a long press.
func TestPadPressRelease(t *testing.T) {
	var got []key
	var pressed, toggled []int
	s := NewStationsWidget(func(i int, on bool) { toggled = append(toggled, i) },
		func(id int, long bool) { got = append(got, key{id, long}) },
		func(i int) { pressed = append(pressed, i) })
	clock := time.Unix(0, 0)
	s.now = func() time.Time { return clock }
	s.Draw(*termforge.NewCanvas(termforge.NewGrid(60, 12)))
	px := ledCols + 4 // pad origin; each key is 3 cells wide, +1 is its centre

	click := func(x, y int, hold time.Duration) {
		s.Click(x, y)
		clock = clock.Add(hold)
		s.Release(x, y)
	}
	click(px+9+1, 2, 100*time.Millisecond)  // →
	click(px+12+1, 2, 100*time.Millisecond) // L: same key as →
	click(px+0+1, 2, 100*time.Millisecond)  // H: left
	click(px+6+1, 0, 100*time.Millisecond)  // K: up
	click(px+6+1, 4, 100*time.Millisecond)  // J: down
	click(px+6+1, 2, 100*time.Millisecond)  // [O]
	click(px+6+1, 2, 600*time.Millisecond)  // [O] held: long
	click(px+6+1, 1, 600*time.Millisecond)  // ↑ held: long

	want := []key{{3, false}, {3, false}, {2, false}, {1, false}, {4, false}, {0, false}, {0, true}, {1, true}}
	if len(got) != len(want) {
		t.Fatalf("got %v, want %v", got, want)
	}
	for i := range want {
		if got[i] != want[i] {
			t.Fatalf("got %v, want %v", got, want)
		}
	}

	s.Click(px+9+1, 2) // press only: nothing yet
	if len(got) != len(want) {
		t.Fatal("key fired on press")
	}
	s.Release(0, 0) // released elsewhere: the pad key still fires (Qt grabs the mouse)
	if len(got) != len(want)+1 {
		t.Fatal("key lost when released off the widget")
	}

	click(ledCols+1, 2, 50*time.Millisecond) // [P] of station 3
	s.Click(ledCols+1, 4)                    // [P] of station 5, released elsewhere: no click
	s.Release(0, 0)
	s.Click(1, 5)     // LED part of station 6 toggles connected on press
	s.PressStation(7) // keyboard 8: same as a click on [P]
	s.Draw(*termforge.NewCanvas(termforge.NewGrid(60, 12)))
	if !s.press[7].Selected {
		t.Fatal("[P] not shown pressed")
	}
	clock = clock.Add(flashFor)
	s.Draw(*termforge.NewCanvas(termforge.NewGrid(60, 12)))
	if s.press[7].Selected {
		t.Fatal("[P] flash did not end")
	}
	if len(pressed) != 2 || pressed[1] != 7 {
		t.Fatalf("pressed %v", pressed)
	}
	pressed = pressed[:1]
	if len(pressed) != 1 || pressed[0] != 2 || len(toggled) != 1 || toggled[0] != 5 {
		t.Fatalf("pressed %v toggled %v", pressed, toggled)
	}
}

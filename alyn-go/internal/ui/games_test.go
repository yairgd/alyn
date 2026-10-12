package ui

import (
	"testing"

	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"
)

func TestGamesPanAndClick(t *testing.T) {
	g := NewGamesWidget()
	picked := -1
	g.SetGames([]string{"a.lua", "b.lua", "c.lua", "d.lua", "e.lua"}, func(i int) { picked = i })
	pane := termforge.NewCanvas(termforge.NewGrid(20, 10)).WithRect(termforge.NewRect(5, 2, 10, 3))

	g.Draw(pane)
	g.Click(6, 2) // top row, not panned
	if picked != 0 {
		t.Fatalf("picked %d, want 0", picked)
	}

	for range 5 { // the wheel stops at the end of the list: 5 labels, 3 rows
		g.HandleEvent(tcell.NewEventMouse(6, 2, tcell.WheelDown, 0))
		g.Draw(pane)
	}
	g.Click(6, 2) // top row now shows label 2
	if picked != 2 {
		t.Fatalf("after pan picked %d, want 2", picked)
	}

	picked = -1
	g.Click(6, 5) // below the pane: a clipped label must not hit
	if picked != -1 {
		t.Fatalf("click outside the pane picked %d", picked)
	}
}

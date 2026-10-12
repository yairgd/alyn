package ui

import (
	"strings"

	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"
)

// GamesWidget lists the games, one Label each. The labels are painted at full size into a
// canvas whose origin is the scroll position (the tradedeck portfolio pattern), so the wheel
// pans the list and the pane clips it.
type GamesWidget struct {
	termforge.BaseWidget
	labels []*termforge.Label
	view   *termforge.RectViewport
	pane   termforge.Rect // screen rect of the pane at the last Draw
}

func NewGamesWidget() *GamesWidget {
	return &GamesWidget{
		BaseWidget: termforge.BaseWidget{PaneName: "Games"},
		view:       termforge.NewRectViewport(),
	}
}

// SetGames fills the list; pick runs with the index of the clicked game.
func (g *GamesWidget) SetGames(names []string, pick func(idx int)) {
	g.labels = g.labels[:0]
	for i, n := range names {
		l := termforge.NewLabel(" " + strings.TrimSuffix(n, ".lua"))
		l.Active = tcell.StyleDefault.Reverse(true)
		l.OnClick = func() { pick(i) }
		g.labels = append(g.labels, l)
	}
	g.view.SetContentSize(0, len(g.labels))
}

// SetActive marks the running game (-1 none).
func (g *GamesWidget) SetActive(idx int) {
	for i, l := range g.labels {
		l.Selected = i == idx
	}
}

// Click selects the game under the screen point (x, y). The app calls it on a press.
func (g *GamesWidget) Click(x, y int) {
	if !g.pane.Contains(x, y) {
		return // a label scrolled out of the pane can still hold its old rect
	}
	for _, l := range g.labels {
		if l.Click(x, y) {
			return
		}
	}
}

func (g *GamesWidget) Draw(c termforge.Canvas) {
	g.pane = c.Rect()
	g.view.Clamp(c.W(), c.H())
	cv := c.WithRect(c.Rect()).WithOrigin(g.view.Origin) // pane is the clip before the origin moves
	for i, l := range g.labels {
		l.Draw(cv.WithRect(cv.ChildRect(0, i, c.W(), 1)))
	}
}

// The wheel pans the list.
func (g *GamesWidget) HandleEvent(ev tcell.Event) {
	m, ok := ev.(*tcell.EventMouse)
	if !ok {
		return
	}
	switch b := m.Buttons(); {
	case b&tcell.WheelUp != 0:
		g.view.Pan(0, -1)
	case b&tcell.WheelDown != 0:
		g.view.Pan(0, 1)
	}
}

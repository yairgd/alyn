package ui

import (
	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"

	"github.com/yairgd/alyn-go/internal/sim"
)

// StationsWidget lists the 8 station LEDs. A blinking LED alternates between
// filled and empty with the MCU's blink phase; a disconnected one shows an X.
// Clicking a line toggles connected (the Qt LED widget did the same).
type StationsWidget struct {
	termforge.BaseWidget
	st                  [sim.Stations]sim.Station
	originX, originY, w int // screen rect from the last Draw
	onToggle            func(station int, on bool)
}

func NewStationsWidget(onToggle func(station int, on bool)) *StationsWidget {
	return &StationsWidget{
		BaseWidget: termforge.BaseWidget{PaneName: "Stations"},
		onToggle:   onToggle,
	}
}

// SetFrame runs on the UI thread.
func (s *StationsWidget) SetFrame(f sim.Frame) { s.st = f.Stations }

func (s *StationsWidget) HandleEvent(tcell.Event) {}

// Click toggles the station under screen point (x, y); the app calls it on a
// primary press (the tree consumes presses for focus, so the widget never sees them).
func (s *StationsWidget) Click(x, y int) {
	i := y - s.originY
	if x < s.originX || x >= s.originX+s.w || i < 0 || i >= sim.Stations || s.onToggle == nil {
		return
	}
	s.onToggle(i, !s.st[i].Connected)
}

func (s *StationsWidget) Draw(c termforge.Canvas) {
	s.originX, s.originY, s.w = c.ScreenX(0), c.ScreenY(0), c.W()
	for i, st := range s.st {
		color := tcell.NewRGBColor(int32(st.R), int32(st.G), int32(st.B))
		glyph, style := '●', tcell.StyleDefault.Foreground(color)
		switch {
		case !st.Connected:
			glyph, style = '✕', tcell.StyleDefault.Foreground(tcell.ColorGray)
		case st.Blink && !st.On:
			glyph = '○'
		}
		c.Printf(0, i, tcell.StyleDefault, " %d ", i+1)
		c.SetContent(3, i, glyph, style)
	}
	c.Print(0, sim.Stations+1, tcell.StyleDefault.Foreground(tcell.ColorGray), " 1-8 button")
	c.Print(0, sim.Stations+2, tcell.StyleDefault.Foreground(tcell.ColorGray), " arrows/Enter keys")
	c.Print(0, sim.Stations+3, tcell.StyleDefault.Foreground(tcell.ColorGray), " H J K L O long")
}

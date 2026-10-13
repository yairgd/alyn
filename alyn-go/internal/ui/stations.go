package ui

import (
	"time"

	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"

	"github.com/yairgd/alyn-go/internal/sim"
)

// StationsWidget lists the 8 station LEDs. A blinking LED alternates between
// filled and empty with the MCU's blink phase; a disconnected one shows an X.
// Clicking a line toggles connected (the Qt LED widget did the same).
// ledCols is the width of the " n ●" part of a row, the click target that toggles connected;
// the [P] button follows it.
const ledCols = 5

type StationsWidget struct {
	termforge.BaseWidget
	st                  [sim.Stations]sim.Station
	originX, originY, w int // screen rect from the last Draw
	onToggle            func(station int, on bool)
	pad                 []padKey // MCU key buttons
	press               [sim.Stations]*termforge.Label
	onKey               func(id int, long bool)
	onPress             func(station int)
	downKey             int       // pad key under the press, -1 none
	downStation         int       // [P] under the press, -1 none
	downAt              time.Time // when the press began
	now                 func() time.Time
	flashUntil          [sim.Stations]time.Time // [P] shown pressed until then
	pan                 termforge.Point         // content cell shown at the pane's top-left
	pane                termforge.Rect          // screen rect of the pane at the last Draw
}

// contentH is the full height of the stations content (LED rows, pad and hint lines).
const contentH = sim.Stations + 2

// longPress is the Qt ArrawKey threshold: a key held this long (press to release) is a long
// press, shorter is a normal press.
const longPress = 500 * time.Millisecond

// flashFor is how long a [P] button shows as pressed after it fired.
const flashFor = 200 * time.Millisecond

// padKey is one clickable key of the pad: an MCU key id, short or long press, at a cell
// offset from the pad origin.
type padKey struct {
	label  *termforge.Label
	dx, dy int
}

// Like the Qt ArrawKey/P buttons, the pad keys act on release: held < 500 ms is a press, longer
// a long press. onKey runs with the MCU key id (0 ok, 1 up, 2 left, 3 right, 4 down).
// onPress runs with the station of a [P] button clicked (pressed and released on it).
func NewStationsWidget(onToggle func(station int, on bool), onKey func(id int, long bool), onPress func(station int)) *StationsWidget {
	s := &StationsWidget{
		BaseWidget: termforge.BaseWidget{PaneName: "Stations"},
		onToggle:   onToggle,
		onKey:      onKey,
		onPress:    onPress,
		downKey:    -1, downStation: -1,
		now: time.Now,
	}
	for i := range s.press {
		i := i
		l := termforge.NewLabel("[P]")
		l.Style = tcell.StyleDefault.Foreground(tcell.ColorWhite).Background(tcell.ColorDarkSlateGray).Bold(true)
		l.Active = tcell.StyleDefault.Foreground(tcell.ColorBlack).Background(tcell.ColorWhite).Bold(true)
		l.OnClick = func() { s.downStation, s.downAt = i, s.now() }
		s.press[i] = l
	}
	short := tcell.StyleDefault.Foreground(tcell.ColorWhite).Background(tcell.ColorDarkSlateGray).Bold(true)
	long := tcell.StyleDefault.Foreground(tcell.ColorSilver).Background(tcell.ColorDarkSlateGray)
	for _, k := range []struct {
		text   string
		id     int
		isLong bool
		dx, dy int
	}{
		{" K ", 1, true, 6, 0}, {" ↑ ", 1, false, 6, 1},
		{" H ", 2, true, 0, 2}, {" ← ", 2, false, 3, 2}, {"[O]", 0, false, 6, 2}, {" → ", 3, false, 9, 2}, {" L ", 3, true, 12, 2},
		{" ↓ ", 4, false, 6, 3}, {" J ", 4, true, 6, 4},
	} {
		k := k
		l := termforge.NewLabel(k.text)
		l.Style = short
		if k.isLong {
			l.Style = long
		}
		l.OnClick = func() { s.downKey, s.downAt = k.id, s.now() }
		s.pad = append(s.pad, padKey{l, k.dx, k.dy})
	}
	return s
}

// SetFrame runs on the UI thread.
func (s *StationsWidget) SetFrame(f sim.Frame) { s.st = f.Stations }

// ScrollBy moves the content up (negative) or down by n rows, clamped to the pane.
func (s *StationsWidget) ScrollBy(n int) {
	s.pan.Y = max(0, min(s.pan.Y+n, contentH-s.pane.H()))
}

// The wheel scrolls the content; every hit rect (LED rows, [P], pad) is taken from the scrolled
// Draw, so clicks follow it.
func (s *StationsWidget) HandleEvent(ev tcell.Event) {
	m, ok := ev.(*tcell.EventMouse)
	if !ok {
		return
	}
	switch b := m.Buttons(); {
	case b&tcell.WheelUp != 0:
		s.ScrollBy(-1)
	case b&tcell.WheelDown != 0:
		s.ScrollBy(1)
	}
}

// Click handles a primary press at screen point (x, y); the app calls it (the tree consumes
// presses for focus, so the widget never sees them). A LED toggles connected at once, as in Qt;
// the pad keys and [P] buttons only note the press, Release acts.
func (s *StationsWidget) Click(x, y int) {
	s.downKey, s.downStation = -1, -1
	if !s.pane.Contains(x, y) {
		return // a row scrolled out of the pane can still hold its old rect
	}
	for _, l := range s.press {
		if l.Click(x, y) {
			return
		}
	}
	for _, k := range s.pad {
		if k.label.Click(x, y) {
			return
		}
	}
	i := y - s.originY
	if x < s.originX || x >= s.originX+ledCols || i < 0 || i >= sim.Stations || s.onToggle == nil {
		return
	}
	s.onToggle(i, !s.st[i].Connected)
}

// Release ends a press begun by Click; the app calls it when the primary button goes up at (x, y).
func (s *StationsWidget) Release(x, y int) {
	if s.downKey >= 0 {
		s.onKey(s.downKey, s.now().Sub(s.downAt) >= longPress)
	}
	if i := s.downStation; i >= 0 && s.pane.Contains(x, y) && s.press[i].Rect().Contains(x, y) {
		s.PressStation(i)
	}
	s.downKey, s.downStation = -1, -1
}

// PressStation presses station i's button, the same as a click on its [P]; the keyboard's 1-8
// call it too.
func (s *StationsWidget) PressStation(i int) {
	s.onPress(i)
	s.flashUntil[i] = s.now().Add(flashFor)
}

func (s *StationsWidget) Draw(c termforge.Canvas) {
	s.pane = c.Rect()
	s.ScrollBy(0)                              // re-clamp after a resize
	c = c.WithRect(c.Rect()).WithOrigin(s.pan) // pane is the clip before the origin moves
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
		s.press[i].Selected = s.now().Before(s.flashUntil[i])
		s.press[i].Draw(c.WithRect(c.ChildRect(ledCols, i, 3, 1)))
	}
	s.drawPad(c, ledCols+4)
	c.Print(0, sim.Stations+1, tcell.StyleDefault.Foreground(tcell.ColorGray), " 1-8 button")
}

// drawPad draws the MCU keys as labels. Each arm (letter + arrow) is one key; hold it for a long press.
func (s *StationsWidget) drawPad(c termforge.Canvas, x int) {
	for _, k := range s.pad {
		k.label.Draw(c.WithRect(c.ChildRect(x+k.dx, k.dy, 3, 1)))
	}
	gray := tcell.StyleDefault.Foreground(tcell.ColorGray)
	c.Print(x, 6, gray, "click: press, hold 0.5s: long")
	c.Print(x, 7, gray, "keys: arrows/Enter, H J K L O long")
}

package ui

import (
	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"

	"github.com/yairgd/alyn-go/internal/sim"
)

// MatrixWidget draws the 64x32 LED canvas, two pixels per cell: '▀' with the
// upper pixel as foreground and the lower one as background. Colours are the
// real RGB (the Qt simulator showed R and B swapped).
type MatrixWidget struct {
	termforge.BaseWidget
	rgb  []byte
	pan  termforge.Point // content cell shown at the pane's top-left
	pane termforge.Rect  // pane rect at the last Draw
	dx   int             // cells per pixel horizontally at the last Draw
}

func NewMatrixWidget() *MatrixWidget {
	return &MatrixWidget{BaseWidget: termforge.BaseWidget{PaneName: "Matrix"}}
}

// SetFrame runs on the UI thread.
func (m *MatrixWidget) SetFrame(f sim.Frame) { m.rgb = f.RGB }

// ScrollBy pans the matrix by (dx, dy) cells, clamped so it never leaves the pane.
func (m *MatrixWidget) ScrollBy(dx, dy int) {
	m.pan.X = max(0, min(m.pan.X+dx, sim.MatrixW*max(m.dx, 1)-m.pane.W()))
	m.pan.Y = max(0, min(m.pan.Y+dy, sim.MatrixH/2-m.pane.H()))
}

// The wheel pans the matrix when it is larger than the pane (as in the stations pane).
func (m *MatrixWidget) HandleEvent(ev tcell.Event) {
	e, ok := ev.(*tcell.EventMouse)
	if !ok {
		return
	}
	switch b := e.Buttons(); {
	case b&tcell.WheelUp != 0:
		m.ScrollBy(0, -1)
	case b&tcell.WheelDown != 0:
		m.ScrollBy(0, 1)
	case b&tcell.WheelLeft != 0:
		m.ScrollBy(-2, 0)
	case b&tcell.WheelRight != 0:
		m.ScrollBy(2, 0)
	}
}

func (m *MatrixWidget) pixel(x, y int) tcell.Color {
	i := (y*sim.MatrixW + x) * 3
	if m.rgb == nil || i+2 >= len(m.rgb) {
		return tcell.ColorBlack
	}
	return tcell.NewRGBColor(int32(m.rgb[i]), int32(m.rgb[i+1]), int32(m.rgb[i+2]))
}

func (m *MatrixWidget) Draw(c termforge.Canvas) {
	dx := 1
	if c.W() >= 2*sim.MatrixW {
		dx = 2 // square-ish pixels when there is room
	}
	m.pane, m.dx = c.Rect(), dx
	m.ScrollBy(0, 0)                           // re-clamp after a resize
	c = c.WithRect(c.Rect()).WithOrigin(m.pan) // pane is the clip before the origin moves
	// centre the canvas in the pane
	ox := max(0, (c.W()-sim.MatrixW*dx)/2)
	oy := max(0, (c.H()-sim.MatrixH/2)/2)
	for cy := 0; cy < sim.MatrixH/2; cy++ {
		for x := 0; x < sim.MatrixW; x++ {
			st := tcell.StyleDefault.Foreground(m.pixel(x, 2*cy)).Background(m.pixel(x, 2*cy+1))
			for k := 0; k < dx; k++ {
				c.SetContent(ox+x*dx+k, oy+cy, '▀', st)
			}
		}
	}
}

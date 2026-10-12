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
	rgb []byte
}

func NewMatrixWidget() *MatrixWidget {
	return &MatrixWidget{BaseWidget: termforge.BaseWidget{PaneName: "Matrix"}}
}

// SetFrame runs on the UI thread.
func (m *MatrixWidget) SetFrame(f sim.Frame) { m.rgb = f.RGB }

func (m *MatrixWidget) HandleEvent(tcell.Event) {}

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
	for cy := 0; cy < sim.MatrixH/2; cy++ {
		for x := 0; x < sim.MatrixW; x++ {
			st := tcell.StyleDefault.Foreground(m.pixel(x, 2*cy)).Background(m.pixel(x, 2*cy+1))
			for k := 0; k < dx; k++ {
				c.SetContent(x*dx+k, cy, '▀', st)
			}
		}
	}
}

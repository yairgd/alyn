package ui

import (
	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"
	"github.com/yairgd/termforge/platform"
)

// HelpOverlay is a floating, scrollable window drawn over the pane tree. It owns no layout
// space: the app gives it a centered rect, so opening it never reshapes the splits. The frame
// is painted cell by cell so it does not merge into the pane separators underneath.
type HelpOverlay struct {
	termforge.BaseWidget
	doc     *termforge.ScrollDocument
	buf     *platform.Buffer
	title   string
	visible bool
}

func NewHelpOverlay() *HelpOverlay {
	buf := platform.NewBuffer()
	doc := termforge.NewScrollDocument(buf)
	doc.SetReadOnly(true)
	doc.SetCursorVisible(false)
	return &HelpOverlay{BaseWidget: termforge.BaseWidget{PaneName: "help"}, doc: doc, buf: buf}
}

func (o *HelpOverlay) Visible() bool { return o.visible }

// Show replaces the contents and opens the window scrolled to the top.
func (o *HelpOverlay) Show(title string, lines []string) {
	o.title = title
	o.buf.Clear()
	for _, l := range lines {
		o.buf.AppendLine(l)
	}
	o.doc.Home()
	o.visible = true
}

func (o *HelpOverlay) Hide() { o.visible = false }

func (o *HelpOverlay) SetClipboard(io termforge.ClipboardIO) { o.doc.SetClipboard(io) }

func (o *HelpOverlay) Draw(c termforge.Canvas) {
	if !o.visible || c.W() < 4 || c.H() < 3 {
		return
	}
	frame := tcell.StyleDefault.Foreground(tcell.ColorSteelBlue)
	c.Fill(' ', tcell.StyleDefault) // opaque: cover what the panes painted here
	w, h := c.W(), c.H()
	for x := 1; x < w-1; x++ {
		c.SetContent(x, 0, '─', frame)
		c.SetContent(x, h-1, '─', frame)
	}
	for y := 1; y < h-1; y++ {
		c.SetContent(0, y, '│', frame)
		c.SetContent(w-1, y, '│', frame)
	}
	c.SetContent(0, 0, '┌', frame)
	c.SetContent(w-1, 0, '┐', frame)
	c.SetContent(0, h-1, '└', frame)
	c.SetContent(w-1, h-1, '┘', frame)
	if o.title != "" {
		c.Print(2, 0, frame.Bold(true), " "+o.title+" ")
	}
	c.Print(2, h-1, frame, " Esc/q close · j/k, PgUp/PgDn, wheel scroll ")

	inner := c.WithRect(c.ChildRect(2, 1, w-4, h-2))
	o.doc.SetMouseOrigin(inner.ScreenX(0), inner.ScreenY(0))
	o.doc.Draw(inner)
}

func (o *HelpOverlay) HandleEvent(ev tcell.Event) {
	if !o.visible {
		return
	}
	switch e := ev.(type) {
	case *tcell.EventMouse:
		o.doc.HandleEvent(e)
	case *tcell.EventKey:
		o.doc.HandleEvent(e)
	}
}

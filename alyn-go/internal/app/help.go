package app

import (
	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"
)

const (
	helpMaxWidth  = 78
	helpMaxHeight = 30
)

var helpLines = []string{
	"Keys (normal mode, the default; Esc leaves insert mode)",
	"  i              type into the Lua console (insert mode)",
	"  :              command line",
	"  Arrows, Enter  MCU keys: up, down, left, right, ok",
	"  K J H L O      long press of up, down, left, right, ok",
	"  1 - 8          station buttons P1-P8",
	"  C-w j / k      focus the pane below / above",
	"  ? or F1        this window",
	"  C-d            quit",
	"",
	"Mouse",
	"  click a game name     start that game",
	"  click a station LED   toggle connected; [P] = its button",
	"  click a pad key       MCU key; hold 0.5 s = long press",
	"  drag a separator      resize the panes",
	"  wheel                 scroll the games list or the console",
	"",
	"Commands",
	"  :games                 list the games, the running one is marked *",
	"  :start <game|n>        run a game by name or number",
	"  :stop                  stop the game",
	"  :load <file.lua>       run a Lua file as the game",
	"  :connect <1-8> [on|off]  set a station connected / not",
	"  :restart               stop the game, new Lua state",
	"  :help  :quit",
	"",
	"Lua console",
	"  Lua 5.4 with the game library: game.clean(), game.plot(x, y, 0xff0000),",
	"  game.delay(us), game.keys(), game.led_rgb(...), ...",
	"",
	"── About ──────────────────────────────────────────────────────────",
	"alyn-go: terminal UI for the alyn LED game, built on termforge.",
	"The MCU C code and Lua VM are alyn's, linked in unchanged.",
}

// helpRect centers the help window over the workspace, leaving a margin.
func helpRect(c termforge.Canvas) termforge.Rect {
	w, h := min(c.W()-8, helpMaxWidth), min(c.H()-4, helpMaxHeight)
	if w < 12 || h < 5 {
		return termforge.Rect{} // too small to frame: Draw skips it
	}
	return c.ChildRect((c.W()-w)/2, (c.H()-h)/2, w, h)
}

func (a *App) OnHelp(args ...any) {
	a.help.Show("help", helpLines)
	a.RequestFrame()
}

func (a *App) hideHelp() {
	a.help.Hide()
	a.RequestFrame()
}

// helpKey: the help window is the top surface, so it owns the keyboard while open.
func (a *App) helpKey(ev *tcell.EventKey) bool {
	if ev.Key() == tcell.KeyEscape || (ev.Key() == tcell.KeyRune && ev.Rune() == 'q') {
		a.hideHelp()
		return true
	}
	if ev.Key() == tcell.KeyRune { // vi keys; the document scrolls on arrows
		switch ev.Rune() {
		case 'j':
			ev = tcell.NewEventKey(tcell.KeyDown, 0, 0)
		case 'k':
			ev = tcell.NewEventKey(tcell.KeyUp, 0, 0)
		}
	}
	a.help.HandleEvent(ev)
	a.RequestFrame()
	return true
}

// helpMouse reports whether the help window took the pointer.
func (a *App) helpMouse(ev *tcell.EventMouse, press bool) bool {
	if !a.help.Visible() {
		return false
	}
	x, y := ev.Position()
	switch {
	case a.WidgetRect(a.help).Contains(x, y):
		a.help.HandleEvent(ev)
		a.RequestFrame()
	case press:
		a.hideHelp() // clicking away closes it
	}
	return true
}

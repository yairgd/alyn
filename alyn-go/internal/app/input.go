package app

import (
	tcell "github.com/gdamore/tcell/v2"

	"github.com/yairgd/termforge"
	"github.com/yairgd/termforge/commands"
	"github.com/yairgd/termforge/platform"

	"github.com/yairgd/alyn-go/internal/sim"
)

func (a *App) HandleMouse(ev *tcell.EventMouse) {
	x, y := ev.Position()
	primary := ev.Buttons()&tcell.ButtonPrimary != 0
	// A held button is a drag (separator resize, text selection), not a new press.
	press := primary && !a.mouseDown
	if !primary && a.mouseDown {
		a.stations.Release(x, y)
	}
	a.mouseDown = primary
	if a.helpMouse(ev, press) {
		return
	}
	inCmd := a.tree.PinnedBottomRect().Contains(x, y)

	if a.Mode() == platform.ModeCommand || a.Mode() == platform.ModeCompletion {
		if primary && inCmd {
			a.cmdWidget.SetCursorAtLocalX(x - a.tree.PinnedBottomRect().X())
			a.RequestFrame()
			return
		}
		if primary {
			a.leaveCommandMode()
		} else {
			return
		}
	}
	if primary && inCmd {
		a.enterCommandMode()
		return
	}
	if press {
		a.stations.Click(x, y)
		a.gameList.Click(x, y)
		a.tree.FocusAt(x, y)
	}
	a.tree.HandleEvent(ev)
	a.RequestFrame()
}

func (a *App) HandleInterrupt(ev *tcell.EventInterrupt) {
	switch m := ev.Data().(type) {
	case termforge.SubmitMsg:
		if a.ctx.Bus != nil {
			a.ctx.Bus.Dispatch(m)
		}
	case sim.Frame:
		a.matrix.SetFrame(m)
		a.stations.SetFrame(m)
		a.gameList.SetActive(a.runningGame())
	case sim.Output:
		a.console.Write(m.Text)
	case sim.Prompt:
		a.console.SetPrompt(m.Text)
	case consoleEnded:
		a.onConsoleEnded()
	}
}

func (a *App) HandleTTYResume() {}

func (a *App) handleNormalKey(ev *tcell.EventKey) bool {
	if a.help.Visible() {
		return a.helpKey(ev)
	}
	a.tryKeyBindings(a.keyBindings, ev)
	return true
}

func (a *App) handleInsertKey(ev *tcell.EventKey) bool {
	if a.help.Visible() {
		return a.helpKey(ev)
	}
	if a.tryKeyBindings(a.insertKeys, ev) {
		return true
	}
	if w := a.tree.FocusedWidget(); w != nil {
		w.HandleEvent(ev)
	}
	return true
}

func (a *App) handleCommandKey(ev *tcell.EventKey) bool {
	a.cmdWidget.HandleEvent(ev)
	if ev.Key() == tcell.KeyEnter {
		a.cmdWidget.Deativate()
		if a.Mode() == platform.ModeCommand {
			a.SetMode(platform.ModeNormal)
		}
	}
	return true
}

func (a *App) tryKeyBindings(reg *commands.KeyBindingRegistry, ev *tcell.EventKey) bool {
	key, ok := platform.KeyFromEvent(ev)
	if !ok {
		reg.ResetPartial()
		return false
	}
	completed, handled := reg.HandleKey(key)
	if !handled {
		return false
	}
	if !completed {
		return reg.InPartial()
	}
	return true
}

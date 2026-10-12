package app

import (
	"github.com/yairgd/termforge/commands"
	"github.com/yairgd/termforge/platform"

	"github.com/yairgd/alyn-go/internal/sim"
)

// The console feeds Lua's REPL, where ':' and most letters are typed text, so keys
// reach the PTY only in insert mode (Esc leaves it, 'i' returns).
func (a *App) InitKeyBindings() {
	a.keyBindings = commands.NewKeyBindingRegistry()
	a.insertKeys = commands.NewKeyBindingRegistry()

	a.keyBindings.Bind(
		commands.NewCommand("move-up", func(args ...any) { a.tree.FocusUp(); a.RequestFrame() }),
		"<C-w>k", "<C-w><Up>",
	)
	a.keyBindings.Bind(
		commands.NewCommand("move-down", func(args ...any) { a.tree.FocusDown(); a.RequestFrame() }),
		"<C-w>j", "<C-w><Down>",
	)
	// MCU keys: 0 ok, 1 up, 2 left, 3 right, 4 down (game-controller buttons.c);
	// the terminal can't time a hold, so long presses are the capitals.
	for _, k := range []struct {
		id    int
		short string
		long  string
	}{{0, "<Enter>", "O"}, {1, "<Up>", "K"}, {2, "<Left>", "H"}, {3, "<Right>", "L"}, {4, "<Down>", "J"}} {
		k := k
		a.keyBindings.Bind(commands.NewCommand("key", func(args ...any) { a.key(k.id, false) }), k.short)
		a.keyBindings.Bind(commands.NewCommand("long-key", func(args ...any) { a.key(k.id, true) }), k.long)
	}
	for i := 0; i < 8; i++ {
		i := i
		a.keyBindings.Bind(commands.NewCommand("button", func(args ...any) {
			a.stations.PressStation(i)
		}), string(rune('1'+i)))
	}
	a.keyBindings.Bind(commands.NewCommand("help", a.OnHelp), "?", "<F1>")
	a.insertKeys.Bind(commands.NewCommand("help", a.OnHelp), "<F1>")
	a.keyBindings.Bind(
		commands.NewCommand("insert", func(args ...any) { a.enterInsert() }),
		"i",
	)
	a.keyBindings.Bind(
		commands.NewCommand("command-mode", func(args ...any) { a.enterCommandMode() }),
		":",
	)
	a.keyBindings.Bind(
		commands.NewCommand("quit", func(args ...any) { a.ForceQuit() }),
		"<C-d>",
	)
	a.insertKeys.Bind(
		commands.NewCommand("escape", func(args ...any) { a.onEscape() }),
		"<Esc>",
	)
}

func (a *App) enterInsert() {
	a.tree.SetInsertActive(true)
	a.SetMode(platform.ModeInsert)
	a.RequestFrame()
}

func (a *App) onEscape() {
	a.tree.SetInsertActive(false)
	a.SetMode(platform.ModeNormal)
	a.RequestFrame()
}

func (a *App) enterCommandMode() {
	a.tree.SetInsertActive(false)
	if !a.cmdWidget.Active() {
		a.cmdWidget.Activate()
	}
	a.SetMode(platform.ModeCommand)
	a.RequestFrame()
}

func (a *App) leaveCommandMode() {
	a.cmdWidget.Deativate()
	a.SetMode(platform.ModeNormal)
	a.RequestFrame()
}

func (a *App) key(id int, long bool) { sim.Key(id, long) }

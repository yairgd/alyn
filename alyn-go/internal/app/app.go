// Package app is the alyn TUI: pane wiring, input routing and commands.
package app

import (
	"github.com/yairgd/termforge"
	"github.com/yairgd/termforge/commands"
	"github.com/yairgd/termforge/platform"

	"github.com/yairgd/alyn-go/internal/sim"
	"github.com/yairgd/alyn-go/internal/ui"
)

// App is the host. termforge.App owns the screen, event loop and mode machine.
type App struct {
	*termforge.App
	commandReg  *commands.CommandRegistry
	keyBindings *commands.KeyBindingRegistry
	insertKeys  *commands.KeyBindingRegistry

	tab       *termforge.TabWidget
	tree      *termforge.WidgetTree
	cmdWidget *termforge.CmdWidget
	ctx       platform.AppContext

	matrix    *ui.MatrixWidget
	stations  *ui.StationsWidget
	gameList  *ui.GamesWidget
	help      *ui.HelpOverlay
	console   *ui.ConsoleWidget
	games     []string
	current   int  // index of the last started game, -1 none
	mouseDown bool // primary button held: a drag, not a new press

	replRunning bool // lua_main is running on its thread
	restart     bool // start a new REPL when the current one ends
}

func New() (*App, error) {
	a := &App{
		App:        termforge.NewApp(),
		commandReg: commands.NewCommandRegistry(),
		current:    -1,
	}
	a.App.Api = a
	if err := a.Init(); err != nil {
		a.Close()
		return nil, err
	}
	return a, nil
}

func (a *App) Close() {
	sim.Stop()
	if a.App != nil {
		a.App.Close()
	}
}

func (a *App) Layout() *termforge.WidgetTree { return a.tree }

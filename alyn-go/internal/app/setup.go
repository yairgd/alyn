package app

import (
	"github.com/yairgd/termforge"
	"github.com/yairgd/termforge/platform"

	"github.com/yairgd/alyn-go/internal/sim"
	"github.com/yairgd/alyn-go/internal/ui"
)

func (a *App) Init() error {
	a.ctx = platform.NewAppContext()

	a.cmdWidget = termforge.NewCmdWidget(a.commandReg)
	a.cmdWidget.Ctx = a.ctx
	a.cmdWidget.SetPostInterrupt(a.PostInterrupt)
	a.cmdWidget.SetClipboard(a.ClipboardIO())
	a.SetCmdline(a.cmdWidget)

	a.console = ui.NewConsoleWidget(sim.SendLine, sim.ConsoleEOF)
	a.console.SetClipboard(a.ClipboardIO())

	a.matrix = ui.NewMatrixWidget()

	a.stations = ui.NewStationsWidget(func(i int, on bool) {
		sim.Connect(i, on)
	}, a.key, sim.Button)

	a.gameList = ui.NewGamesWidget()
	a.games = sim.Games()
	a.gameList.SetGames(a.games, a.startGame)

	//	+--------+----------+
	//	| matrix | stations |
	//	|        |----------|
	//	|        |  games   |
	//	+--------+----------+
	//	|       console     |
	//	+-------------------+
	a.tree = termforge.NewWidgetTree(a.matrix)
	a.tree.SetEqualAlways(true)
	a.tree.Split(termforge.Horizontal, a.console)
	a.tree.FocusWidget(a.matrix)
	a.tree.Split(termforge.Vertical, a.stations)
	a.tree.FocusWidget(a.stations)
	a.tree.Split(termforge.Horizontal, a.gameList)
	a.tree.SetEqualAlways(false)
	if root := a.tree.Root(); root != nil && root.Type == termforge.NodeSplit {
		root.Ratio = 2.0 / 3 // matrix row over console
		if top := root.First; top != nil && top.Type == termforge.NodeSplit {
			top.Ratio = 2.0 / 3 // matrix beside stations/games
			if col := top.Second; col != nil && col.Type == termforge.NodeSplit {
				col.Ratio = 0.62 // stations over games
			}
		}
	}
	a.tree.FocusWidget(a.console)
	a.tree.SetStatusClipboard(a.ClipboardIO())
	a.tree.SetOnResize(a.RequestFrame)
	a.tree.SetEqualAlways(false)
	a.tree.PinBottom(a.cmdWidget, 1)
	a.tab = termforge.NewTabWidget("alyn", a.tree)
	a.AddWidget(a.tab)

	a.help = ui.NewHelpOverlay()
	a.help.SetClipboard(a.ClipboardIO())
	a.AddFloatingWidget(a.help, helpRect)

	a.InitKeyBindings()
	a.InitCommands()
	a.RegisterModeHandler(platform.ModeNormal, a.handleNormalKey)
	a.RegisterModeHandler(platform.ModeInsert, a.handleInsertKey)
	a.RegisterModeHandler(platform.ModeCommand, a.handleCommandKey)

	sim.Init(a.PostInterrupt)
	a.startConsole()
	// normal mode: the MCU keys and 1-8 work at once; 'i' types into the Lua console
	return nil
}

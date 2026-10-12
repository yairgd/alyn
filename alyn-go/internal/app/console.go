package app

import "github.com/yairgd/alyn-go/internal/sim"

type consoleEnded struct{}

// startConsole runs the Lua REPL (C thread, see internal/sim) until EOF.
func (a *App) startConsole() {
	a.replRunning = true
	go func() {
		sim.RunConsole()
		a.PostInterrupt(consoleEnded{})
	}()
}

func (a *App) onConsoleEnded() {
	a.replRunning = false
	if a.restart {
		a.restart = false
		a.startConsole()
		return
	}
	a.console.Write("[Lua console ended; :restart]\n")
	a.RequestFrame()
}

// Restart stops the game and gives the REPL a fresh Lua state.
func (a *App) Restart(args ...any) {
	sim.Stop()
	if !a.replRunning {
		a.startConsole()
		return
	}
	a.restart = true
	sim.ConsoleEOF()
}

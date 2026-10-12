package app

import (
	"fmt"
	"os"
	"strconv"
	"strings"

	"github.com/yairgd/alyn-go/internal/sim"
)

func (a *App) InitCommands() {
	a.commandReg.Root.
		LeafRestComplete("start", a.OnStart, a.gameCompletions).
		Leaf("stop", a.OnStop).
		Leaf("help", a.OnHelp).
		Leaf("games", a.OnGames).
		LeafRest("load", a.OnLoad).
		LeafRest("connect", a.OnConnect).
		Leaf("restart", a.Restart).
		Leaf("quit", a.Quit)
}

func (a *App) Quit(args ...any) { a.Exit() }

func (a *App) ForceQuit(args ...any) { a.Exit() }

// OnStart runs an embedded game, by name or index.
func (a *App) OnStart(args ...any) {
	arg := argString(args)
	if n, err := strconv.Atoi(arg); err == nil {
		a.startGame(n)
		return
	}
	for i, g := range a.games {
		if g == arg || strings.TrimSuffix(g, ".lua") == arg {
			a.startGame(i)
			return
		}
	}
	a.console.Note("no such game: " + arg + "\n")
}

func (a *App) OnStop(args ...any) {
	sim.Stop()
	a.current = -1
}

// runningGame is the index of the game that is running now, or -1.
func (a *App) runningGame() int {
	if sim.Running() {
		return a.current
	}
	return -1
}

func (a *App) startGame(i int) {
	if sim.Start(i) {
		a.current = i
	}
}

// OnGames lists the embedded games; the running one is marked.
func (a *App) OnGames(args ...any) {
	var b strings.Builder
	b.WriteString("games:\n")
	for i, g := range a.games {
		mark := "  "
		if i == a.runningGame() {
			mark = "* "
		}
		fmt.Fprintf(&b, "%s%d %s\n", mark, i, strings.TrimSuffix(g, ".lua"))
	}
	a.console.Note(b.String())
}

func (a *App) gameCompletions(prefix string, _ bool) []string {
	var out []string
	for _, g := range a.games {
		if strings.HasPrefix(g, strings.TrimSpace(prefix)) {
			out = append(out, g)
		}
	}
	return out
}

// OnConnect: ":connect <1-8> [on|off]" sets a station's connected state.
func (a *App) OnConnect(args ...any) {
	f := strings.Fields(argString(args))
	if len(f) == 0 {
		return
	}
	n, err := strconv.Atoi(f[0])
	if err != nil || n < 1 || n > sim.Stations {
		a.console.Note("connect: station 1-8\n")
		return
	}
	on := len(f) < 2 || f[1] != "off"
	sim.Connect(n-1, on)
}

// argString joins the rest-args (they arrive split on whitespace).
func argString(args []any) string {
	var parts []string
	for _, a := range args {
		if s, ok := a.(string); ok {
			parts = append(parts, s)
		}
	}
	return strings.TrimSpace(strings.Join(parts, " "))
}

// OnLoad: ":load file.lua" runs a Lua file as the game (the MCU's "game in memory").
func (a *App) OnLoad(args ...any) {
	path := argString(args)
	src, err := os.ReadFile(path)
	if err != nil {
		a.console.Note("load: " + err.Error() + "\n")
		return
	}
	sim.Load(string(src))
}

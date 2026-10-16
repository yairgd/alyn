# alyn-go

termforge TUI replacing the Qt GUI of the alyn LED game simulator
(`../game-pc/led_matrix_simulator`). alyn-go lives inside the alyn repo and depends on it: the MCU C code
is built in place from `../common` (incl. its `luaEffects/lua-5.4.6`; `ALYN_ROOT` in `native/CMakeLists.txt`
defaults to the repo root). Never copy it here or edit it from alyn-go. It is linked into the Go binary
through cgo (`internal/sim`); Lua's console hooks (`USE_GUI_CONSOLE ON`) feed the termforge console widget.
The repo README has a short "alyn-go" section (what it is, build/run).
Architecture: **`claude-docs/ARCHITECTURE.md`**. Build/progress log: `claude-docs/PROGRESS.md`. Writing new Lua effects: `claude-docs/LUA_EFFECTS.md`.

## Commands

```bash
task build        # C libs (CMake -> lib/) + Go TUI (cgo) into bin/
task run          # the TUI
task check        # build + vet + test + gofmt
```

`native:configure` needs `-DCMAKE_INSTALL_PREFIX` (the Taskfile sets it); use the task, not a bare cmake.

## Rules

- Minimal changes, no new layers. termforge comes from the module cache (the `replace` in go.mod is commented out;
  `GOFLAGS=-mod=mod go mod download github.com/yairgd/termforge@<ver>` fills go.sum if it is missing).
- Never `git add`/`git commit`; suggest them.
- `go build` needs `lib/` from `task native:build`.
- Test the TUI headlessly in tmux (`new-session -d`, `send-keys`, `capture-pane`). tmux can't
  deliver mouse events; use a python pty and write `\x1b[<0;X;YM` for clicks.

## Things that bit us (read before changing the UI)
- **Mouse presses never reach widgets**: `WidgetTree` consumes them for focus. `App.HandleMouse` routes them to
  `stations.Click` / `gameList.Click` on press and to `stations.Release` on button-up (`mouseDown` tracks it).
  termforge `Label.Click` only hits after its first `Draw` (it stores its screen rect).
- **Start mode is normal.** In insert mode every key goes to the Lua console, so `H J K L`, arrows and `1`-`8`
  look dead. `i` types into the console, `Esc` returns.
- **Only `game_config.lua` reads keys** (`game.keys()`); the other games ignore them. In its config screen a
  long ok (`O`) starts the blink selector, short ok stops it, `->` before `O` starts the game.
- **Pad keys act on release like Qt `ArrawKey`**: held < 500 ms = `set_key`, longer = `set_long_key`, never both.
  A letter (`H J K L`) is the same key as its arrow. `[P]` fires only if pressed and released on it (Qt `clicked`).
  The LED part of a row toggles connected on press (Qt `LedCircle`).
- Keyboard `1`-`8` and a click on `[P]` both call `StationsWidget.PressStation`.
- cgo is Go -> C only. C threads never call Go; output goes through a pipe, input through a condition variable
  (`native/src/console.c`).
- Mouse and layout checks need a real pty; unit-test widgets with `termforge.NewCanvas(termforge.NewGrid(w, h))`
  and a fake `now` (see `internal/ui/stations_test.go`). `go test` prints a harmless `tmpnam` linker warning.
- Not fixed: `set_long_key` in `common/utils/system_model.c:350` uses `key_pad` instead of `long_key_pad`
  (shared MCU code, ask before touching).

# alyn-go architecture

One Go process. The alyn MCU C code (`common/`) is linked in as static libraries through cgo,
the way the Qt simulator linked it into its own process:

```
bin/alyn  (Go, termforge)
  internal/sim  --cgo-->  libalyn_sim.a + lua, lua_module, lua_binaries, common_static,
                          luaEffects_static, effects_static, Hal   (lib/, built from alyn common/)
     C threads: tick (25 ms), game (Lua), REPL (lua_main on a locked OS thread)
```

## Native (`native/`)
- `CMakeLists.txt` sets what alyn's top-level CMake provides (`TARGET=LINUX_PC`, `LUAC_PATH`, ...), then
  `add_subdirectory($ALYN_ROOT/common)` (`ALYN_ROOT` defaults to the enclosing repo, `../..` from `native/`) and installs the
  static libs into `lib/`. The host `luac` + `lua_fw_to_c.py` pipeline there builds the embedded games.
- **`USE_GUI_CONSOLE ON`**: `lua.c` calls the external `readline(prompt)` / `add_history`, and
  `print`/errors go to `write_line` / `write_format_line` (lauxlib.h). Same hooks the Qt
  `terminaltextedit.cpp` implements; here `src/console.c` does:
  - output -> chunks `kind u8 | len u32 LE | bytes` (`O` text, `P` prompt) on a pipe, written with
    cancellation disabled (the game thread prints too and is `pthread_cancel`ed);
  - `readline` writes the prompt chunk then blocks on a condition until Go calls `console_send_line`;
    `console_eof` makes it return NULL, ending `lua_main`.
  No Go callbacks are made from C threads.
- `src/sim.c`: tick thread (`timer_manage`, `manage_blink(0..7)`), game thread (`luaL_newstate`..`lua_pcall`,
  stopped by `pthread_cancel`; game errors go to the console), `sim_frame` (merged channel 3 + station LEDs),
  key/button/connect inputs. Port of `mainwindow.cpp` + `threadedworker.cpp`.

## Go
- `internal/sim`: cgo wrapper. `Init(post)` starts a 25 ms frame ticker and the console pipe reader;
  both `post` (= `App.PostInterrupt`) `Frame`, `Output`, `Prompt`. `RunConsole()` is `lua_main` on a locked thread.
  The test (`go test ./internal/sim`) drives REPL, keys, `Load`, `Stop`, `Connect` end to end.
- `internal/app`: tradedeck-style app. `console.go` starts/restarts the REPL goroutine; `HandleInterrupt` routes
  the messages to the widgets (UI thread only).
- `internal/ui`: `MatrixWidget` (`▀` half blocks, real RGB), `StationsWidget` (8 LEDs; the app routes
  the click because `WidgetTree` consumes presses), `GamesWidget` (one `termforge.Label` per game, painted at full size into a canvas with `WithOrigin(view.Origin)` and a `RectViewport`; wheel pans, click on a label starts the game, the app routes the press like stations), `ConsoleWidget` (xterm view, local echo, no PTY;
  copy of gdbforge's Lua console pattern with a dynamic prompt so `>>` continuation lines work).
- The `WidgetTree` must be wrapped in a `TabWidget` before `AddWidget`, else nothing is drawn.

## Input
Console takes keys only in insert mode (Lua uses `:`); `Esc` -> normal. Normal mode: arrows/Enter =
MCU keys 1,4,2,3,0 (up, down, left, right, ok); `K J H L O` = long presses; `1`-`8` = station buttons;
`C-w j/k` focus. Mouse: click focuses a pane or toggles a station; drag a separator to resize (matrix rescales);
wheel and drag-select work in the console. `App.HandleMouse` tracks `mouseDown` so a drag is not a new press.
Help: `:help`, `?` (normal mode) or F1 opens a floating window (`ui.HelpOverlay`, copy of the termforge demo's, which is internal) with keys, mouse, commands and a short About at the end; it owns keyboard and pointer while open. Text is `helpLines` in `internal/app/help.go`.
Commands: `:games` (list, running one marked `*`), `:start <game|n>`, `:stop`, `:load <file.lua>`, `:connect <1-8> [on|off]`,
`:restart` (stop game, new Lua state), `:quit`.

## Limits / known issues
- All C state is process-global: one world per process (the test is one big test for that reason).
- `lua.c` resets SIGINT to default after the first statement; the terminal is raw so Ctrl-C is a key,
  but an external `kill -INT` would kill the process.
- Lua `io.write` goes to the real stdout (not the console); `print` and errors are in the console.
- alyn bug, not fixed: `set_long_key` uses `key_pad` instead of `long_key_pad` (`common/utils/system_model.c:350`).

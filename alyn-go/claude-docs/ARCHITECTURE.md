# alyn-go architecture

Summary: a terminal LED simulator, equivalent to the Qt one in `game-pc/led_matrix_simulator`. One Go process links the
repo's MCU C code and Lua 5.4.6 (cgo), shows the 64x32 matrix, 8 station LEDs with `[P]` buttons, a clickable MCU key pad,
a game list and the Lua REPL. Sections: Native, Go, Lua integration, Stations pane, Input, Limits.

One Go process. The alyn MCU C code (`common/`) is linked in as static libraries through cgo,
the way the Qt simulator linked it into its own process:

```
bin/alyn  (Go, termforge)
  internal/sim  --cgo-->  libalyn_sim.a + lua, lua_module, lua_binaries, common_static,
                          luaEffects_static, effects_static, Hal   (lib/, built from alyn common/)
     C threads: tick (25 ms), game (Lua), REPL (lua_main on a locked OS thread)
```

Location: `alyn/alyn-go/` inside the alyn repo; it uses the repo's `common/` (and `common/luaEffects/lua-5.4.6`) in place.

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
- `internal/ui`: `MatrixWidget` (`▀` half blocks, real RGB), `StationsWidget` (8 LEDs, wheel-scrolled via a pan origin like tradedeck's TableWindow, + a clickable MCU key pad of `termforge.Label`s; the app routes
  the click because `WidgetTree` consumes presses), `GamesWidget` (one `termforge.Label` per game, painted at full size into a canvas with `WithOrigin(view.Origin)` and a `RectViewport`; wheel pans, click on a label starts the game, the app routes the press like stations), `ConsoleWidget` (xterm view, local echo, no PTY;
  copy of gdbforge's Lua console pattern with a dynamic prompt so `>>` continuation lines work).
- The `WidgetTree` must be wrapped in a `TabWidget` before `AddWidget`, else nothing is drawn.

## Lua integration (how Go reaches Lua)
- Lua 5.4.6 is the patched copy in `common/luaEffects/lua-5.4.6`, built as `liblua.a`. `linit.c` opens the extra `game`
  library (`common/luaEffects/src/lgamelib.c`: `game.plot/blink/keys/...`). `CONFIG_LUA_GAME_LIB` renames `main` to `lua_main`.
- Output: `lauxlib.h` maps `lua_writestring/lua_writestringerror` to `write_line/write_format_line`, defined in
  `native/src/console.c` (not in Lua). They write `kind u8 | len u32 LE | bytes` chunks to a pipe; `sim.console()` reads it
  and posts `sim.Output` / `sim.Prompt` to the UI thread (`App.PostInterrupt` -> `HandleInterrupt`).
- Input: `lua.c` `lua_readline` -> `readline(prompt)` (console.c) writes the prompt chunk and blocks on a condition;
  `ConsoleWidget.submit` -> `sim.SendLine` -> `console_send_line` wakes it. `sim.ConsoleEOF` ends `lua_main`.
- Games: `.lua` files are embedded by `luac` + `lua_fw_to_c.py` (`luasrc.h`). `sim_game_start(idx)` / `sim_game_start_source`
  (`:load`) run them on a pthread with their own `lua_State` (`game_main`); the REPL is a second state on a locked OS thread.
- Frames: a Go ticker (25 ms) calls `C.sim_frame`; keys/buttons/connect call `sim_key/sim_button/sim_connect`.
- cgo: each `C.f()` becomes a generated `_Cfunc_f` wrapper + C stub (`_cgo_runtime_cgocall`); no C -> Go callbacks.
- The console widget is termforge `CompositeTerminal` -> `TerminalController` -> `xterm-go` (`gitpod-io/xterm-go`,
  indirect dependency): output via `WriteRaw`, local echo via `SetInputHandler`, the typed line is read back from the screen buffer.

## Stations pane (`internal/ui/stations.go`)
```
 1 ● [P]        K        row: " n ●" toggles connected (press), [P] presses the station button
 2 ● [P]        ↑        pad: K/↑ up, H/← left, [O] ok, →/L right, ↓/J down
 3 ● [P]  H  ← [O] →  L  each key is a termforge.Label; click = press, hold >= 500 ms = long press
```
`NewStationsWidget(onToggle, onKey, onPress)`. `Click(x,y)` (press) records the pad key / `[P]` under the pointer and the time;
`Release(x,y)` fires (`onKey(id, held>=500ms)`, `PressStation(i)` if released on the same `[P]`). `[P]` shows
white-on-black for 200 ms after firing. The matrix is centred in its pane (`MatrixWidget.Draw`).

## Input
The app starts in normal mode. The console takes keys only in insert mode (`i`) (Lua uses `:`); `Esc` -> normal. Normal mode: arrows/Enter =
MCU keys 1,4,2,3,0 (up, down, left, right, ok); `K J H L O` = long presses; `1`-`8` = station buttons;
`C-w j/k` focus. Mouse: click focuses a pane, starts a game, toggles a station, presses a pad key / `[P]`; drag a separator to
resize (matrix rescales); wheel and drag-select work in the console. `App.HandleMouse` tracks `mouseDown` so a drag is not a new
press and sends `stations.Release` on button-up.
Help: `:help`, `?` (normal mode) or F1 opens a floating window (`ui.HelpOverlay`, copy of the termforge demo's, which is internal) with keys, mouse, commands and a short About at the end; it owns keyboard and pointer while open. Text is `helpLines` in `internal/app/help.go`.
Commands: `:games` (list, running one marked `*`), `:start <game|n>`, `:stop`, `:load <file.lua>`, `:connect <1-8> [on|off]`,
`:restart` (stop game, new Lua state), `:quit`.

## Limits / known issues
- All C state is process-global: one world per process (the test is one big test for that reason).
- `lua.c` resets SIGINT to default after the first statement; the terminal is raw so Ctrl-C is a key,
  but an external `kill -INT` would kill the process.
- Lua `io.write` goes to the real stdout (not the console); `print` and errors are in the console.
- alyn bug, not fixed: `set_long_key` uses `key_pad` instead of `long_key_pad` (`common/utils/system_model.c:350`).

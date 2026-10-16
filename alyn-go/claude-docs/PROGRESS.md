# alyn-go progress

Update after every step and before stopping. Never git add/commit (suggest only).

## Summary
alyn-go is a termforge TUI that replaces the Qt LED-matrix simulator. It lives inside the alyn repo and builds against the
repo's own `common/` (MCU C code + `common/luaEffects/lua-5.4.6`) in place, nothing copied. The C code is linked into the Go
binary through cgo with `USE_GUI_CONSOLE ON`, so there is no external process, PTY or socket.

Done:
- Native: `native/CMakeLists.txt` (`ALYN_ROOT` = enclosing repo) builds the static libs into `lib/`; `native/src/{sim,console}.c`
  = tick thread, game thread, REPL console hooks, frame, key/button/connect inputs.
- Go: `internal/sim` (cgo wrapper + end-to-end test), `internal/app` (commands, keys, mouse, help), `internal/ui`
  (matrix, stations + MCU key pad, games list, console, help overlay).
- Commands: `:games :start :stop :load :connect :restart :help :quit`.
- Matrix shown as `▀` half blocks, real RGB (no Qt R/B swap), centred in its pane.
- Stations pane: clickable MCU key pad and a `[P]` button per LED (Qt semantics: act on release, hold >= 500 ms = long).
- Games pane: one `termforge.Label` per game, wheel pans, click starts it.
- Layout: matrix pane starts at 2/3 of width and 2/3 of height (split ratios in `internal/app/setup.go`).
- Eight new Lua effects/games in `common/luaEffects/src/luasrc/` (registered in that dir's `CMakeLists.txt`, an
  agreed exception to "don't edit common"): plasma, fire, starfield, matrix_rain, life, lava, snake, tetris.
  How to write more (API, key bits, quirks, headless testing): **`LUA_EFFECTS.md`** - read it first.
- `task check` (build + vet + test + gofmt) passes.

## History
- 2026-10-12: first design (separate `alyn-mcu` process + PTY + socket protocol) replaced the same day by the in-process cgo
  design; `alyn-mcu`, `internal/mcu`, ptyx removed.
- 2026-10-12: console, matrix, stations, games pane, help window, drag-aware mouse, matrix centring, key pad added.
- 2026-10-12: moved into the alyn repo; README section added.
- 2026-10-12: `ALYN_ROOT` default fixed to the enclosing repo (`../..` from `native/`); docs use repo-relative paths; termforge
  v0.6.0 fetched into the module cache (go.sum was missing it). Clean rebuild and `task check` pass.
- 2026-10-16: matrix pane 2/3 x 2/3; added the 8 Lua effects/games above; wrote `LUA_EFFECTS.md`.

## Gotchas
- Edited `.lua` files are not rebuilt reliably by `task build`, and Go ignores a changed `.a`: use
  `go build -a -o bin/alyn ./cmd/alyn` and `go test -a`.
- `native:configure` needs `-DCMAKE_INSTALL_PREFIX` (the Taskfile sets it); `go build` needs `lib/` from `task native:build`.
- The `WidgetTree` must be wrapped in a `TabWidget` before `AddWidget`, else a blank screen.
- `WidgetTree.HandleEvent` consumes mouse presses, so the app routes clicks to stations/games itself.
- Rest-args of commands arrive split on whitespace (join them).
- tmux `send-keys` cannot deliver mouse events; use a python pty and write `\x1b[<0;X;YM`.
- Only warning in the build: `tmpnam` in Lua's `loslib.c`.
- termforge `Label` (`label.go`) was added in `/home/yair/tui/termforge` (generic, tested); that repo may still need the commit.

## Open issues
- alyn bug, not fixed: `set_long_key` uses `key_pad` instead of `long_key_pad` (`common/utils/system_model.c:350`);
  only `game_config.lua` reads keys.
- `go.mod` has the termforge `replace` commented out (uses v0.6.0 from the module cache).
- alyn-go is not part of the top-level CMake build; use `task build`.

## Suggested commits
From the repo root: `git add README.md alyn-go` (bin/, lib/, native/build/ are ignored).

## Next ideas
More Lua games (pong, breakout, kaleidoscope); Go test for app wiring; drive the real board over serial; optional top-level CMake target.

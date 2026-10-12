# alyn-go progress

Plan: `/home/yair/.claude/plans/i-want-to-convert-eventual-unicorn.md`
Update after every step and before stopping. Never git add/commit (suggest only).

## Checklist
- [x] 1. Native skeleton: native/CMakeLists.txt + `alyn-mcu --headless` (REPL, game 0, frame checksum)
- [x] 2. Console via PTY: Go app skeleton + ConsoleWidget + ptyx.Start
- [x] 3. Socket + matrix: link.c / internal/mcu, HELLO/FRAME/START/STOP, MatrixWidget
- [x] 4. Input + stations: KEY/BUTTON/CONNECT, StationsWidget
- [x] 5. Polish: :load, :restart, CLAUDE.md, claude-docs/ARCHITECTURE.md, row in /home/yair/tui/CLAUDE.md

## Log
- 2026-10-12: plan approved; created alyn-go/ and this file.
- 2026-10-12: phase 1 done. native/CMakeLists.txt + src/{main,sim}.c. `cmake -S native -B native/build/Debug` + `cmake --build ... --target alyn-mcu` link first try, alyn repo untouched. `alyn-mcu --headless`: 9 games listed, REPL `print(1+1)` -> 2, game 0 runs, frame checksum changes each second. Only warning: tmpnam in loslib.
- 2026-10-12: phase 2 done. Taskfile/go.mod/cmd/alyn/internal/{app,ui}. `task build` -> bin/alyn + bin/alyn-mcu. Verified in tmux: console shows Lua banner, `print(1+1)` -> 2, Esc then `:quit` exits with no leftover alyn-mcu. Gotcha: the WidgetTree must be wrapped in a TabWidget before AddWidget (else blank screen). Gotcha: native configure needs -DCMAKE_INSTALL_PREFIX (Taskfile does it). Test harness: tmux new-session -d + send-keys + capture-pane (pyte not installed).
- 2026-10-12: phase 3 done. native link.c/link.h (+sim_stations), internal/mcu {proto,link,id}.go + tests, ui/matrix.go, :start <game|idx>, :stop, C-w j/k. Verified in tmux: `:start bouncing_balls` animates with real RGB pixels, `:stop` freezes, `:quit` leaves no process and no socket file. Protocol constants mirrored in native/src/link.h and internal/mcu/proto.go.
- 2026-10-12: phase 4 done. ui/stations.go, key bindings (arrows/Enter short; K J H L O long; 1-8 buttons), `:connect n [on|off]`, click toggles station. Verified: game.keys() == 2 after Up (key id 1); `:connect 2 off` -> X; click via python pty -> X. Gotchas: rest-args arrive split on whitespace (join them); WidgetTree.HandleEvent consumes mouse presses (App routes the click to StationsWidget); tmux send-keys can't deliver mouse events.
- 2026-10-12: phase 5 done. `:load file.lua` verified (red line drawn), `:restart`, CLAUDE.md, claude-docs/ARCHITECTURE.md, row in /home/yair/tui/CLAUDE.md, .gitignore. `task check` passes.
- 2026-10-12: REDESIGN per user: no external process. Lua + MCU C code are linked into the Go binary (cgo) with `USE_GUI_CONSOLE ON`; Lua's readline/write_line hooks (native/src/console.c) feed a PTY-less local-echo ConsoleWidget through a pipe + condition. Removed alyn-mcu, socket protocol, ptyx, internal/mcu. New: internal/sim (cgo) with an end-to-end test (REPL, keys, load, stop/pthread_cancel under Go, connect), `lib/` install of the static libs. Verified in tmux: REPL, `>>` continuation, error traceback, `:start`, `:restart`, `:quit` clean (no leftover process). Stale alyn-mcu processes/sockets from old test runs had to be killed by hand.
- 2026-10-12: added `:games` (ConsoleWidget.Note keeps the prompt) and drag-aware mouse (press vs drag via mouseDown). Verified: separator drag via python pty moved it 71 -> 59 as dragged; `:games` marks the running game.
- 2026-10-12: GamesWidget (stations over games, right of the matrix). Added `termforge/label.go` + `label_test.go` (generic Label: Draw, Click, Rect). Verified via python pty: wheel pans the list (moving_text appears), click on a name starts that game with the pan offset applied (`:games` shows `*3 demo1`). Unit test internal/ui/games_test.go. termforge tests pass.

## Suggested commits (termforge, not run)
git -C /home/yair/tui/termforge add label.go label_test.go
git -C /home/yair/tui/termforge commit -m "feat(label): add clickable Label"
- 2026-10-12: help window (`:help`, `?`, F1) + About text. Verified in tmux: opens, PgDn reaches About, q closes, quit clean.

## Open issues / decisions
- (superseded) first design was a separate alyn-mcu process + PTY. Now in-process via cgo, USE_GUI_CONSOLE ON.
- Real RGB (no Qt R/B swap).

## Suggested commits
cd /home/yair/tui/alyn-go && git init   # not a repo yet
git add .gitignore Taskfile.yml go.mod go.sum cmd internal native/CMakeLists.txt native/src
git commit -m "feat: alyn-go TUI with native alyn-mcu"
(CLAUDE.md and claude-docs/ stay local if this becomes a public repo; ask which.)

## Next
All phases done. Ideas: README/docs, Go test for app wiring, drive the real board over serial.

# alyn-go

termforge TUI replacing the Qt GUI of the alyn LED game simulator
(`../game-pc/led_matrix_simulator`). The MCU C code stays C and is built
in place from the enclosing repo's `../common` (incl. `luaEffects/lua-5.4.6`) (never copy or edit it from here), linked into the Go binary
through cgo (`internal/sim`); Lua's console hooks (`USE_GUI_CONSOLE ON`) feed the termforge console widget.
Architecture: **`claude-docs/ARCHITECTURE.md`**. Build/progress log: `claude-docs/PROGRESS.md`.

## Commands

```bash
task build        # C libs (CMake -> lib/) + Go TUI (cgo) into bin/
task run          # the TUI
task check        # build + vet + test + gofmt
```

`native:configure` needs `-DCMAKE_INSTALL_PREFIX` (the Taskfile sets it); use the task, not a bare cmake.

## Rules

- Minimal changes, no new layers . termforge comes from the module cache (the `replace` in go.mod is commented out).
- Never `git add`/`git commit`; suggest them.
- `go build` needs `lib/` from `task native:build`.
- Test the TUI headlessly in tmux (`new-session -d`, `send-keys`, `capture-pane`). tmux can't
  deliver mouse events; use a python pty and write `\x1b[<0;X;YM` for clicks.

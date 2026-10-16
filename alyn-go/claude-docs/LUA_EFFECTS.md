# Writing Lua effects for the game list

Notes from the session that added `plasma`, `fire` and `starfield`. Read this before generating more.

## Where effects live
- Source: `../common/luaEffects/src/luasrc/<name>.lua` (shared MCU code, so edits there are exceptions to the
  "don't touch common" rule; keep them additive).
- Register: add `${CMAKE_CURRENT_SOURCE_DIR}/luasrc/<name>.lua` to `lua_source_files` in
  `../common/luaEffects/src/CMakeLists.txt`. The game list shows the name with `.lua` (`plasma.lua`).
- Each file is a standalone script: set up state, then `while true do ... game.delay(us) end`. No return.

## `game.*` API (from `lgamelib.c`)
Colours are `0xRRGGBB` ints; pixel (0,0) is top-left; matrix is 64 wide x 32 high.
- `game.plot(x, y, c)`, `game.line(x1, y1, x2, y2, c)`, `game.circle(x, y, r, c)`, `game.fill_circle(x, y, r, c)`
- `game.clean()` clears all 3 channels; `game.clean_rect(x, y, w, h)`
- `game.rotate(n)` shifts the canvas by n columns (n>0 left, n<0 right; used by `scrolling_color.lua`)
- `game.delay(us)` sleeps in **microseconds** (30000 = ~33 fps)
- `game.print(str, x, y, c)`, `game.select_channel(id)`, `game.opacity(a, b, c)`
- `game.keys()` / `game.long_keys()` key bitmasks (only `game_config.lua` uses them), `game.is_station_connected(i)`
- Floats are fine as coordinates (truncated to int). Lua 5.4: integer ops `<<`, `//` and `math.sin/sqrt/random` work.
- Out-of-range plots appear to be clipped, but bounds-check anyway when cheap.

## Patterns that worked
- Full-screen effects (plasma, fire) overwrite every pixel each frame: no `game.clean()` in the loop, no flicker.
  Build a 256-entry palette table once and index it per pixel (`pal[...]`).
- Sparse effects (starfield, balls) call `game.clean()` at the top of each frame.
- 2048 `game.plot` calls per frame at 30 ms was fine in the simulator.
- Fire: weighted neighbour average must have gain <= 1 (weights 1,2,1,1 divided by 5) plus random cooling
  (`random()*18`); a divisor smaller than the weight sum made everything saturate to white.

## Testing an effect
- `task build` does **not** reliably pick up edited `.lua` files in the embedded list. Rebuild the binary with
  `go build -a -o bin/alyn ./cmd/alyn` and run tests with `go test -a`; Go does not notice the `.a` changed.
- Quick check without rebuilding: `sim.Load(string(src))` in a throwaway test in `internal/sim` (see
  `sim_test.go` for the `Init` / `RunConsole` / frame-callback setup), sleep ~1.5 s, then inspect `Frame.RGB`
  (`(y*MatrixW+x)*3`: R,G,B). Count lit pixels or sample a column. Delete the throwaway test afterwards.
- Lua errors print to the console widget (and into `Output` messages in a test).

## Ideas not done yet
Matrix rain, game of life, kaleidoscope, spinning radar sweep, bouncing DVD logo, lava lamp (metaballs),
sine-wave scroller with `game.print`.

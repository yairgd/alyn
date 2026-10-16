-- Conway's Game of Life on a wrapping torus; cells are coloured by age.
-- Reseeds with a fresh random soup when the colony dies out or gets stuck.
W, H = 64, 32

local function age_color(a)
	if a == 1 then return 0xFFFFFF end
	if a < 4 then return 0x00FFA0 end
	if a < 10 then return 0x0080FF end
	return 0x8030FF
end

local function seed()
	local g = {}
	for i = 0, W * H - 1 do g[i] = math.random() < 0.3 and 1 or 0 end
	return g
end

grid = seed()
stale = 0
prev_pop = -1
game.clean()
while true do
	local nxt, pop = {}, 0
	for y = 0, H - 1 do
		local ym, yp = ((y - 1) % H) * W, ((y + 1) % H) * W
		for x = 0, W - 1 do
			local xm, xp = (x - 1) % W, (x + 1) % W
			local a = grid[y * W + x]
			local n = (grid[ym + xm] > 0 and 1 or 0) + (grid[ym + x] > 0 and 1 or 0) + (grid[ym + xp] > 0 and 1 or 0)
				+ (grid[y * W + xm] > 0 and 1 or 0) + (grid[y * W + xp] > 0 and 1 or 0)
				+ (grid[yp + xm] > 0 and 1 or 0) + (grid[yp + x] > 0 and 1 or 0) + (grid[yp + xp] > 0 and 1 or 0)
			local v = 0
			if a > 0 and (n == 2 or n == 3) then v = a + 1
			elseif a == 0 and n == 3 then v = 1 end
			nxt[y * W + x] = v
			if v > 0 then pop = pop + 1 end
			game.plot(x, y, v > 0 and age_color(v) or 0x000000)
		end
	end
	grid = nxt
	stale = (pop == prev_pop) and stale + 1 or 0
	prev_pop = pop
	if pop < 8 or stale > 25 then grid = seed(); stale = 0 end
	game.delay(80000)
end

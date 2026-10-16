-- Snake. Pad: arrows steer (no U-turns), ok restarts after game over.
-- Field is 30x14 cells of 2x2 pixels inside a 1-pixel frame.
FW, FH = 30, 14
OX, OY = 2, 2
K_OK, K_UP, K_LEFT, K_RIGHT, K_DOWN = 0, 1, 2, 3, 4

local function cell(cx, cy, c)
	local x, y = OX + cx * 2, OY + cy * 2
	game.plot(x, y, c); game.plot(x + 1, y, c)
	game.plot(x, y + 1, c); game.plot(x + 1, y + 1, c)
end

local function frame_box(c)
	for x = 1, 62 do game.plot(x, 1, c); game.plot(x, 30, c) end
	for y = 1, 30 do game.plot(1, y, c); game.plot(62, y, c) end
end

-- returns the pad keys pressed since the last call and clears them
local function poll()
	local k, got = game.keys(), {}
	for i = 0, 4 do
		if k & (1 << i) ~= 0 then got[#got + 1] = i end
		game.set_key(i, 0); game.set_long_key(i, 0)
	end
	return got
end

local function rgb(r, g, b) return r << 16 | g << 8 | b end

local DIRS = {
	[K_UP] = { 0, -1 }, [K_DOWN] = { 0, 1 }, [K_LEFT] = { -1, 0 }, [K_RIGHT] = { 1, 0 },
}

local function place_food(snake)
	while true do
		local fx, fy = math.random(0, FW - 1), math.random(0, FH - 1)
		local free = true
		for _, s in ipairs(snake) do
			if s[1] == fx and s[2] == fy then free = false; break end
		end
		if free then return fx, fy end
	end
end

local function draw(snake, fx, fy, t, dead)
	game.clean()
	frame_box(dead and 0x601010 or 0x203060)
	cell(fx, fy, (t // 3) % 2 == 0 and 0xFF2020 or 0xFF8040)
	local n = #snake
	for i = n, 1, -1 do
		local f = 1 - (i - 1) / math.max(n, 8)
		local c = i == 1 and 0xC0FFC0 or rgb(0, math.floor(60 + 195 * f), math.floor(40 * f))
		if dead then c = rgb(math.floor(60 + 120 * f), 20, 20) end
		cell(snake[i][1], snake[i][2], c)
	end
end

local function play()
	local snake = { { 15, 7 }, { 14, 7 }, { 13, 7 } }
	local dx, dy = 1, 0
	local queue = {}
	local fx, fy = place_food(snake)
	local t, grow = 0, 0
	poll()
	while true do
		-- speed up with length; poll keys several times per step for snappy steering
		local step_ms = math.max(55, 140 - #snake * 3)
		for _ = 1, 4 do
			for _, k in ipairs(poll()) do
				if DIRS[k] and #queue < 2 then queue[#queue + 1] = DIRS[k] end
			end
			game.delay(step_ms * 250)
		end
		if #queue > 0 then
			local d = table.remove(queue, 1)
			if d[1] ~= -dx or d[2] ~= -dy then dx, dy = d[1], d[2] end
		end
		local hx, hy = snake[1][1] + dx, snake[1][2] + dy
		local hit = hx < 0 or hy < 0 or hx >= FW or hy >= FH
		if not hit then
			for i = 1, #snake - (grow > 0 and 0 or 1) do
				if snake[i][1] == hx and snake[i][2] == hy then hit = true; break end
			end
		end
		if hit then draw(snake, fx, fy, t, true); return #snake - 3 end
		table.insert(snake, 1, { hx, hy })
		if hx == fx and hy == fy then
			grow = grow + 2
			fx, fy = place_food(snake)
		end
		if grow > 0 then grow = grow - 1 else table.remove(snake) end
		t = t + 1
		draw(snake, fx, fy, t, false)
	end
end

while true do
	local score = play()
	game.delay(700000)
	game.clean()
	game.print("GAME OVER", 10, 10, 0xFF0000)
	game.print(string.format("SCORE %d", score), 10, 18, 0xFFFF00)
	poll()
	while #poll() == 0 do game.delay(50000) end
end

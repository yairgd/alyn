-- Tetris. Pad: left/right move, up rotate, down soft drop, ok hard drop.
-- Field 10x16 of 2x2 pixel cells (20x32) on the left; next piece and score on the right.
BW, BH = 10, 16
OX = 6
K_OK, K_UP, K_LEFT, K_RIGHT, K_DOWN = 0, 1, 2, 3, 4

-- cells relative to a pivot; rotation is (x, y) -> (-y, x)
SHAPES = {
	I = { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 2, 0 } },
	O = { { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
	T = { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 0, 1 } },
	S = { { 0, 0 }, { 1, 0 }, { -1, 1 }, { 0, 1 } },
	Z = { { -1, 0 }, { 0, 0 }, { 0, 1 }, { 1, 1 } },
	J = { { -1, 0 }, { 0, 0 }, { 1, 0 }, { -1, 1 } },
	L = { { -1, 0 }, { 0, 0 }, { 1, 0 }, { 1, 1 } },
}
NAMES = { "I", "O", "T", "S", "Z", "J", "L" }
COLORS = { I = 0x00E0E0, O = 0xF0E000, T = 0xB030F0, S = 0x30D030, Z = 0xF03030, J = 0x3050F0, L = 0xF09020 }

local function rotated(cells, n)
	local out = cells
	for _ = 1, n % 4 do
		local nx = {}
		for i, c in ipairs(out) do nx[i] = { -c[2], c[1] } end
		out = nx
	end
	return out
end

local bag = {}
local function next_name()
	if #bag == 0 then
		for i, n in ipairs(NAMES) do bag[i] = n end
		for i = #bag, 2, -1 do
			local j = math.random(i)
			bag[i], bag[j] = bag[j], bag[i]
		end
	end
	return table.remove(bag)
end

local function poll()
	local k, got = game.keys(), {}
	for i = 0, 4 do
		if k & (1 << i) ~= 0 then got[#got + 1] = i end
		game.set_key(i, 0); game.set_long_key(i, 0)
	end
	return got
end

local function px(cx, cy, c, ox, oy)
	local x, y = ox + cx * 2, oy + cy * 2
	game.plot(x, y, c); game.plot(x + 1, y, c)
	game.plot(x, y + 1, c); game.plot(x + 1, y + 1, c)
end

local board
local function fits(name, rot, x, y)
	for _, c in ipairs(rotated(SHAPES[name], rot)) do
		local cx, cy = x + c[1], y + c[2]
		if cx < 0 or cx >= BW or cy >= BH then return false end
		if cy >= 0 and board[cy][cx] then return false end
	end
	return true
end

local function draw(cur, nxt, score, lines, flash)
	game.clean()
	for y = 0, 31 do game.plot(OX - 1, y, 0x404060); game.plot(OX + BW * 2, y, 0x404060) end
	for y = 0, BH - 1 do
		for x = 0, BW - 1 do
			local n = board[y][x]
			if n then
				px(x, y, flash and flash[y] and 0xFFFFFF or COLORS[n], OX, 0)
			elseif (x + y) % 2 == 0 then
				game.plot(OX + x * 2, y * 2, 0x101018)
			end
		end
	end
	if cur then
		-- ghost
		local gy = cur.y
		while fits(cur.name, cur.rot, cur.x, gy + 1) do gy = gy + 1 end
		for _, c in ipairs(rotated(SHAPES[cur.name], cur.rot)) do
			if gy + c[2] >= 0 then game.plot(OX + (cur.x + c[1]) * 2, (gy + c[2]) * 2, 0x505050) end
			if cur.y + c[2] >= 0 then px(cur.x + c[1], cur.y + c[2], COLORS[cur.name], OX, 0) end
		end
	end
	-- side panel
	game.print("NEXT", 34, 0, 0xFFFFFF)
	for _, c in ipairs(rotated(SHAPES[nxt], 0)) do px(c[1] + 1, c[2] + 1, COLORS[nxt], 40, 12) end
	game.print(string.format("%d", score), 34, 21, 0xFFFF00)
end

local function play()
	board = {}
	for y = 0, BH - 1 do board[y] = {} end
	local score, lines = 0, 0
	local nxt = next_name()
	local cur
	local tick = 0
	local function spawn()
		cur = { name = nxt, rot = 0, x = 4, y = 0 }
		nxt = next_name()
		return fits(cur.name, 0, cur.x, cur.y)
	end
	if not spawn() then return 0 end
	poll()
	while true do
		local level = lines // 8
		local interval = math.max(2, 14 - level * 2)
		local moved = false
		local hard = false
		for _, k in ipairs(poll()) do
			if k == K_LEFT and fits(cur.name, cur.rot, cur.x - 1, cur.y) then cur.x = cur.x - 1
			elseif k == K_RIGHT and fits(cur.name, cur.rot, cur.x + 1, cur.y) then cur.x = cur.x + 1
			elseif k == K_UP and cur.name ~= "O" then
				for _, kick in ipairs({ 0, -1, 1, -2, 2 }) do
					if fits(cur.name, cur.rot + 1, cur.x + kick, cur.y) then
						cur.rot = (cur.rot + 1) % 4; cur.x = cur.x + kick; break
					end
				end
			elseif k == K_DOWN then tick = interval
			elseif k == K_OK then hard = true end
		end
		if hard then
			while fits(cur.name, cur.rot, cur.x, cur.y + 1) do cur.y = cur.y + 1; score = score + 2 end
			tick = interval
		end
		tick = tick + 1
		if tick >= interval then
			tick = 0
			if fits(cur.name, cur.rot, cur.x, cur.y + 1) then
				cur.y = cur.y + 1
			else
				-- lock
				for _, c in ipairs(rotated(SHAPES[cur.name], cur.rot)) do
					local cx, cy = cur.x + c[1], cur.y + c[2]
					if cy >= 0 then board[cy][cx] = cur.name end
				end
				local full = {}
				local n = 0
				for y = 0, BH - 1 do
					local all = true
					for x = 0, BW - 1 do if not board[y][x] then all = false; break end end
					if all then full[y] = true; n = n + 1 end
				end
				if n > 0 then
					for _ = 1, 3 do
						draw(nil, nxt, score, lines, full); game.delay(70000)
						draw(nil, nxt, score, lines, nil); game.delay(70000)
					end
					local ny = BH - 1
					for y = BH - 1, 0, -1 do
						if not full[y] then board[ny] = board[y]; ny = ny - 1 end
					end
					for y = ny, 0, -1 do board[y] = {} end
					lines = lines + n
					score = score + ({ 100, 300, 500, 800 })[n] * (level + 1)
				end
				if not spawn() then return score end
				cur.y = 0
			end
		end
		draw(cur, nxt, score, lines, nil)
		game.delay(33000)
	end
end

while true do
	local score = play()
	game.delay(500000)
	game.clean()
	game.print("GAME OVER", 10, 9, 0xFF0000)
	game.print(string.format("SCORE %d", score), 10, 17, 0xFFFF00)
	poll()
	while #poll() == 0 do game.delay(50000) end
end

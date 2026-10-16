-- Fire: heat rises from a flickering bottom row, cools as it climbs
W, H = 64, 32

-- black -> red -> orange -> yellow -> white
local function color(h)
	local r = math.min(255, h * 3)
	local g = math.min(255, math.max(0, h * 3 - 255))
	local b = math.min(255, math.max(0, h * 3 - 510))
	return math.floor(r) << 16 | math.floor(g) << 8 | math.floor(b)
end

pal = {}
for i = 0, 255 do pal[i] = color(i) end

heat = {}
for i = 0, W * (H + 1) - 1 do heat[i] = 0 end

game.clean()
while true do
	-- ignite the hidden bottom row
	for x = 0, W - 1 do
		heat[H * W + x] = math.random(120, 255)
	end
	for y = 0, H - 1 do
		for x = 0, W - 1 do
			local l, r = (x - 1) % W, (x + 1) % W
			local v = (heat[(y + 1) * W + l] + heat[(y + 1) * W + x] * 2
				+ heat[(y + 1) * W + r] + heat[math.min(y + 2, H) * W + x]) / 5
			v = math.max(0, v - math.random() * 18)
			heat[y * W + x] = v
			game.plot(x, y, pal[math.floor(math.min(255, v))])
		end
	end
	game.delay(35000)
end

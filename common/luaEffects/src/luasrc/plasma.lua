-- Plasma: overlapping sine waves mapped to a rainbow palette
W, H = 64, 32

local function hsv(h, s, v)
	local i = math.floor(h * 6)
	local f = h * 6 - i
	local p, q, t = v * (1 - s), v * (1 - f * s), v * (1 - (1 - f) * s)
	i = i % 6
	local r, g, b
	if i == 0 then r, g, b = v, t, p
	elseif i == 1 then r, g, b = q, v, p
	elseif i == 2 then r, g, b = p, v, t
	elseif i == 3 then r, g, b = p, q, v
	elseif i == 4 then r, g, b = t, p, v
	else r, g, b = v, p, q end
	return math.floor(r * 255) << 16 | math.floor(g * 255) << 8 | math.floor(b * 255)
end

-- 256-entry palette, built once
pal = {}
for i = 0, 255 do pal[i] = hsv(i / 256, 1, 1) end

game.clean()
t = 0
while true do
	for y = 0, H - 1 do
		for x = 0, W - 1 do
			local v = math.sin(x / 6 + t)
				+ math.sin(y / 4 - t * 1.3)
				+ math.sin((x + y) / 7 + t * 0.7)
				+ math.sin(math.sqrt((x - 32) ^ 2 + (y - 16) ^ 2) / 4 - t * 1.7)
			game.plot(x, y, pal[math.floor((v + 4) / 8 * 255) % 256])
		end
	end
	t = t + 0.12
	game.delay(30000)
end

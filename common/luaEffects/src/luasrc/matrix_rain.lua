-- Matrix rain: green drops with white heads and fading tails
W, H = 64, 32
TAIL = 10

drops = {}
for x = 0, W - 1 do
	drops[x] = { y = -math.random(0, H * 2), speed = 0.3 + math.random() * 0.9, len = math.random(5, TAIL + 6) }
end

local function green(f)  -- f: 1 = brightest
	local g = math.floor(40 + 215 * f * f)
	return (g // 5) << 16 | g << 8 | (g // 4)
end

while true do
	game.clean()
	for x = 0, W - 1 do
		local d = drops[x]
		d.y = d.y + d.speed
		local head = math.floor(d.y)
		for i = 0, d.len do
			local y = head - i
			if y >= 0 and y < H then
				game.plot(x, y, i == 0 and 0xDDFFDD or green(1 - i / (d.len + 1)))
			end
		end
		if head - d.len > H then
			d.y = -math.random(0, 12)
			d.speed = 0.3 + math.random() * 0.9
			d.len = math.random(5, TAIL + 6)
		end
	end
	game.delay(40000)
end

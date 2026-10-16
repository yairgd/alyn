-- Lava lamp: drifting metaballs, glowing from deep red to hot yellow-white
W, H = 64, 32
N = 5

balls = {}
for i = 1, N do
	balls[i] = {
		x = math.random() * W, y = math.random() * H,
		vx = (math.random() - 0.5) * 0.6, vy = (math.random() - 0.5) * 0.5,
		r = 4 + math.random() * 3,
	}
end

pal = {}
for i = 0, 255 do
	local r = math.min(255, i * 2)
	local g = math.min(255, math.max(0, i * 2 - 300))
	local b = math.min(255, math.max(0, i * 3 - 650))
	pal[i] = r << 16 | g << 8 | b
end

while true do
	for _, b in ipairs(balls) do
		b.x, b.y = b.x + b.vx, b.y + b.vy
		if b.x < 0 or b.x > W then b.vx = -b.vx end
		if b.y < 0 or b.y > H then b.vy = -b.vy end
	end
	for y = 0, H - 1 do
		for x = 0, W - 1 do
			local f = 0
			for _, b in ipairs(balls) do
				local dx, dy = x - b.x, (y - b.y) * 1.4
				f = f + b.r * b.r / (dx * dx + dy * dy + 1)
			end
			local v = math.floor(math.max(0, math.min(1, (f - 0.45) * 0.9)) * 255)
			game.plot(x, y, pal[v])
		end
	end
	game.delay(40000)
end

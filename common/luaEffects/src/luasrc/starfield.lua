-- Starfield: fly through space, stars streak and warm up as they approach
W, H = 64, 32
CX, CY = W / 2, H / 2
N = 70

stars = {}
local function respawn(s, far)
	s.x = (math.random() - 0.5) * 2
	s.y = (math.random() - 0.5) * 2
	s.z = far and 1 or math.random()
end
for i = 1, N do stars[i] = {}; respawn(stars[i], false) end

local function shade(z)
	local b = math.floor(255 * (1 - z))
	-- far = cold blue, near = white
	return (b * 3 // 4) << 16 | (b * 7 // 8) << 8 | b
end

while true do
	game.clean()
	for _, s in ipairs(stars) do
		local px, py = CX + s.x / s.z * CX, CY + s.y / s.z * CY
		s.z = s.z - 0.018
		local qx, qy = CX + s.x / s.z * CX, CY + s.y / s.z * CY
		if s.z <= 0.02 or qx < 0 or qx >= W or qy < 0 or qy >= H then
			respawn(s, true)
		else
			local c = shade(s.z)
			game.line(px, py, qx, qy, c)
			game.plot(qx, qy, 0xFFFFFF)
		end
	end
	game.delay(30000)
end

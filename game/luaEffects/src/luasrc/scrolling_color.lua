-- simple color scroll using hsv2rgb in Jinx!Script
-- (c) 2015 Sven Karschewski
-- (c) 2023 Yair Gadelov - porting to lua

step =0.5 
hue =0;
move_direction = 0;  -- left

matrix_x = 64
matrix_y = 32


function hsv2rgb(h, s, v)
	local r, g, b

	local i = math.floor(h * 6)
	local f = h * 6 - i
	local p = v * (1 - s)
	local q = v * (1 - f * s)
	local t = v * (1 - (1 - f) * s)

	i = i % 6

	if i == 0 then
		r, g, b = v, t, p
	elseif i == 1 then
		r, g, b = q, v, p
	elseif i == 2 then
		r, g, b = p, v, t
	elseif i == 3 then
		r, g, b = p, q, v
	elseif i == 4 then
		r, g, b = t, p, v
	else
		r, g, b = v, p, q
	end

	return math.floor(r * 255), math.floor(g * 255), math.floor(b * 255)
end



function init()

	-- color change speed	
	step = 20

	-- set hue to 0
	hue = 0

end

function left() 
	-- move the screen 1px left
	game.rotate(-1)

	-- get rgb values for hsv color by hue
	red, green, blue = hsv2rgb (hue/360, 1,1)
	-- draw line
	game.line(matrix_x-1, 0, matrix_x-1, matrix_y, red * 256 * 256 + green * 256  + blue)
end
function right() 
	-- move the screen 1px left
	game.rotate(1)

	-- get rgb values for hsv color by hue
	red, green, blue = hsv2rgb (hue/360, 1,1)
	-- draw line
	game.line(0, 0, 0, matrix_y, red * 256 * 256 + green * 256  + blue)
end
move =   {[0] = left, [1] = right}

function render(channel_id)
	game.select_channel(channel_id)

	move[move_direction]();
	-- increment hue by step
	hue = hue + step
	if hue > 360  then
		hue = 0
	end

end

init()

--b1=banner1.new(rect.new(10,3,40,15), "2 רפסמ רנאב  test in hebrew  ",1,1,0)

game.clean()

--game.opacity(4,3,3)


step=1
while (true) 
do	
	render(0)

--	b1:render(2);			
	game.delay(100000/2);	
end



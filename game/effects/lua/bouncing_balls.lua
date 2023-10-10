--  Bouncing Balls iN Jinx!Script
-- (c) 2014 Sven Karschewski
-- (c) 2023 Yair Gadelov - porting to Lua


-- initialize
-- size of the balls
radius=2

-- how many balls 
balls=10

-- acceleration on falling
gravity=0.2

-- bounce factor means how much energy is left after a bounce
bounce=0.7

-- maximal horizontal moving speed
x_velocity_max=1.2

-- vertical moving speed
y_velocity=1

-- maximal offset for random start of new balls
max_offset=30

-- only use primary colors for random color ?
primary_color=0

-- use text instead of balls
use_text=0


-- pre calculate x velocity factor for random generator
x_velocity=(x_velocity_max/2)*10

ball_x = {}
ball_vx = {}
ball_y={}
ball_vy={}
matrix_x = 61
matrix_y = 29
ball_red={}
ball_blue={}
ball_green={}
pixelstep=1

-- initialize
function init()	
	--initialize the balls
	for n=1,balls,1
	do

		-- first ball starts immediately, following balls get a random offset
		if (n==1) then
			offset=0
		else
			offset=math.random(max_offset)
		end

		-- ball drops in randomly left or right
		if (math.random(10) < 5) then
			ball_x[n]=-radius-offset			
			ball_vx[n]=(math.random(x_velocity)+x_velocity)/10
		else 
			ball_x[n]=matrix_x+radius+offset
			ball_vx[n]=-(math.random(x_velocity)+x_velocity)/10
		end

		-- drop height of the ball
		ball_y[n]=-radius-offset
		ball_vy[n]=y_velocity

		-- set ball color
		random_color(n)
	end	

end


-- frame generation
function render()

	-- loop over pixelstep
	for ps=1,pixelstep,1
	do
		-- clear the screen	
		game.clean(2,2,62,30)

		-- loop over all balls
		for n=1 ,balls,1
		do

			-- draw the ball
			--if (use_text > 0)
			--	text ball_x[n], ball_y[n], 1, matrix_y/2, ball_red[n], ball_green[n], ball_blue[n], "Jinx!", "Arial", 5
			--else
			--	circle ball_x[n], ball_y[n], radius, ball_red[n], ball_green[n], ball_blue[n], 1
			--endif
			game.fill_circle( ball_x[n], ball_y[n], radius,ball_red[n]<<16 | ball_green[n]<<8 | ball_blue[n] )

			-- lets make some movement
			ball_x[n]=ball_x[n]+ball_vx[n]
			ball_y[n]=ball_y[n]+ball_vy[n]

			-- and add some gravity to the ball
			ball_vy[n]=ball_vy[n]+gravity

			-- bounce if the ball reaches the floor
			if ball_y[n]+radius >= matrix_y-1 then
				ball_y[n]=matrix_y-radius-1
				ball_vy[n]=ball_vy[n]*(-1*bounce)
			end

			-- generate a new ball if the ball moved out of the screen
			if (ball_x[n]-radius > matrix_x+2 and ball_vx[n] > 0) then
				offset=math.random(max_offset)
				ball_x[n]=matrix_x+radius+offset
				ball_y[n]=-radius-offset
				ball_vx[n]=-(math.random(x_velocity)+x_velocity)/10
				ball_vy[n]=y_velocity
				random_color(n)
			elseif (ball_x[n]+radius < -1 and ball_vx[n] < 0) then  
				offset=math.random(max_offset)
				ball_x[n]=-radius-offset
				ball_y[n]=-radius-offset
				ball_vx[n]=(math.random(x_velocity)+x_velocity)/10
				ball_vy[n]=y_velocity
				random_color(n)
			end
		end	

	end	
end


--# new random color, only use primary colors if primary_color is set > 0
function random_color(n)
	if (primary_color > 0) then
		ball_red[n]=0;
		ball_green[n]=0;
		ball_blue[n]=0;
		while (ball_red[n]+ball_green[n]+ball_blue[n] == 0 | ball_red[n]+ball_green[n]+ball_blue[n] == 255*3)
		do
			ball_red[n]=math.random(1)*255
			ball_green[n]=math.random(1)*255
			ball_blue[n]=math.random(1)*255
		end
	else
		ball_red[n]=math.random(255)
		ball_green[n]=math.random(255)
		ball_blue[n]=math.random(255)
	end
end


init()
while (true) 
do
	game.delay(100000/2);
	render()
end

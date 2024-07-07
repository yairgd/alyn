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

-- global variables
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
function render(channel_id)

	game.select_channel(channel_id)
	-- loop over pixelstep
	for ps=1,pixelstep,1
	do
		-- clear the screen	
		--game.clean(2,2,62,30)
--		game.clean(0,0,64,32)

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

-- require("t2")
a1=frame.new(rect.new(0,0,64,32),0xff00ff,0xfff00, 5,1 ,1);
a2=frame.new(rect.new(1,1,62,30),0x000fff,0x0ffff, 10,1 ,1);

b1=banner1.new(rect.new(10,3,40,15), "2 רפסמ רנאב  test in hebrew  ",1,1,0)
b2=banner2.new(rect.new(20,3,40,15), "2 רפסמ רנאב  test in hebrew  ",1,3,500,500,1)
cnt=0;
init()
--b:config(rect.new(10,3,40,15), "reconfig !!! ",1,2,0)
b2:config(rect.new(10,15,40,15), "reconfig !!!!  ",0,2,500,500,1)
a2:config(rect.new(1,1,62,30),0xfff000,0x000fff, 10,1 ,0);

game.opacity(0,0.7,0.3)


game.clean()

while (true) 
do
	game.clean_rect(0,0,64,32)	

	b2:render(1);	
	b1:render(1);	
	
	render(2)
	a1:render(2)
	a2:render(2)
	
	game.delay(100000/4);	
end


a=banner.new()
a:render();



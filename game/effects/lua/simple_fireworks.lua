-- Firework in Jinx!Script
-- algorithm is based on a demo written by Josh83 (josh83@wanadoo.fr)
-- (c) 2014 Sven Karschewski
-- (c) 2023 Yair Gadelov porting to Lua


--gravity for particles 9.8 would be the real world gravity ;)
gravity=9.8
	
--number of particles of the firework
particles=30

--length of particle
particle_length=7
	
--particle fragment distance
particle_step=15

-- particle start speed
particle_start_speed=10

-- only use primary colors for random color ?
primary_color=0

matrix_x = 64
matrix_y = 32

--init global variables 
particle_speed={}
particle_angle={}
restart=1


-- subroutine to create a new explosion
function new_explosion()

	--position of the new explosion
	pos_x=math.random(matrix_x/2)-matrix_x/4
	pos_y=math.random(matrix_y/2)-matrix_y/4

	--create particles
	for n=0 , particles-1 do
		
		--set a random speed for particle movement
		particle_speed[n]=math.random(20)-10+particle_start_speed
		
		--set a random angle for particle movement
		--take a value inside pi as angle and start one out of 5 in the upper direction
		if math.random(4) == 2 then
			particle_angle[n]=math.random(314)*0.01*(-1)
		else
			particle_angle[n]=math.random(314)*0.01
		end
	end	
	--start explosion
	lifetime=particle_length*(-1)
	color_step=0;

	--get new random color
	if primary_color > 0 then
		red=0;
		green=0;
		blue=0;
		while (red+green+blue == 0 or red+green+blue == 255*3) do
			red=math.random(1)*255
			green=math.random(1)*255
			blue=math.random(1)*255
		end
	else
		red=math.random(255)
		green=math.random(255)
		blue=math.random(255)
	end
end

-- routine for initializing
function init()
	-- flag for creating a new explosion on start
	restart=1
	
end


-- main routine for frame generation
function render(channel_id)
	-- create a new explosion
	if restart==1 then
		new_explosion()
		restart=0
	end

	-- clear the matrix
	game.clean(0,0,matrix_x,matrix_y)
	
	-- step over particle length
	for n=0, particle_length*10-1 ,  particle_step do

		--pre calculate time
		time=(lifetime+n*0.1)*0.1
		if (time<0) then
			time=0
		end
			
		--calculate and draw the particle
		for i=0, particles-1 , 1 do
			x=particle_speed[i]*math.cos(particle_angle[i])*time+matrix_x/2+pos_x
			y=matrix_y-(-0.5*gravity*time*time+particle_speed[i]*math.sin(particle_angle[i])*time+matrix_y/2)+pos_y
			game.plot ( x, y, (red-color_step)*256*256 +  (green-color_step) *256+ blue-color_step)
		end
	end
	-- rise the age of the particle
	lifetime=lifetime+1
	color_step=color_step+5

	-- reached lifetime of explosion ?
	if lifetime>=20 then
		-- set flag to start next explosion
		restart=1
	end
end



init()
while (true) 
do	
	render(0)
	game.delay(100000/2);	
end



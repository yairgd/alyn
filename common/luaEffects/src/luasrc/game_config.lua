
--------------------------------------------begin  simple_firewors.lua -------------------------------------------------

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



--------------------------------------------end  simple_firewors.lua -------------------------------------------------

--require("t3")
--
a1=frame.new(rect.new(0,0,64,32),0xff00ff,0x00ff00, 5,1 ,1);
--a2=frame.new(rect.new(1,1,62,30),0xff0000,0x0000ff, 10,1 ,-5);

function print_game_name(i)
	local game_name = { "ףיצר", "יארקא" , "תווצק" ,  "סיסבל הרזח" , "ןועש","                          "}	
	game.print ("          ",3  , 17,0)
	game.print (game_name[i],3 + (60 - (6*game.strlen(game_name[i])))/2 , 17,0)
end

function print_string(str)
	game.print ("          ",3  , 17,0)
	game.print (str,3 + (60 - (6*game.strlen(str)))/2 , 17,0)	
end

function config()
--	a3=frame.new(rect.new(2,2,60,28),0x00ff00,0xff00ff, 20,1 , 0);

	p1=1
	p2=2;
	p3=3;
	--m1="העדוה"
	m2="תוריהמ/ןמז"
	m3="תוציחל"


	game.set_key(0,0)
	game.set_key(1,0)
	game.set_key(2,0)
	game.set_key(3,0)
	game.set_key(4,0)
	game.set_long_key(0,0)
	game.set_long_key(1,0)
	game.set_long_key(2,0)
	game.set_long_key(3,0)
	game.set_long_key(4,0)
	r = rect.new(8,5,62,15)
	r2 = rect.new(18,17,62,15);

	s = string.format("%2d:%2d:%2d",p1,p2,p3)
	state=0; -- no blink
	b=banner2.new(rect.new(8,4,62,15), s,0,1,250,0,0)
--	mb=banner2.new(r2, "       ",1,0,250,0,0)

	-- state: 0 - no blink, 1 (fro mleft)  blink, 2 2 blink 3 blinks 

	while( 1 )
	do
		k = game.keys()
		lk = game.long_keys()
					
		for i=0,4,1
		do
			ck = k&(1<<i);
			lck = lk&(1<<i);
			game.set_long_key(i,0);						
			game.set_key(i,0);


			if (ck>0 or lck>0) then
				--print (ck,state,i)
				if (ck == 4 and state ==0) then
					return p1,p2,p3
				end
			
				if (lck == 1 and state==0) then
						-- begin to blink
						print_game_name(p1)
						state=1
						b:config(r, s,0,1,250,250,0)				
				elseif (ck==1 and state >0 ) then
						-- no blink
						state=0;
						game.print ("          ",3  , 17,0)


						b:config(r, s,0,1,250,0,0)
				end

				if (ck == 8) then
					-- move blink to right
					if (state==1) then
						b:config(r, s,  3,4,250,250,0)
						print_string(m2)
						state=2;
					elseif (state==2) then
						b:config(r, s,6,7,250,250,0)
						print_string(m3)
						state=3;
					elseif (state==3) then
						b:config(r, s,0,1,250,250,0)
						print_game_name(p1)
						state=1;
					end
				end
				if (ck == 4) then
					-- move blink to left
					if (state==3) then
						b:config(r, s,3,4,250,250,0)
						print_string(m2)
						state=2;
					elseif (state==2) then
						b:config(r, s,0,1,250,250,0)
						print_game_name(p1)
						state=1;
					elseif (state==1) then
						b:config(r, s,6,7,250,250,0)
						print_string(m3)
						state=3;
					end
				end
				-- up key
				if (ck==2) then
					if (state==1) then
						p1=p1+1;
						if (p1>5) then 
							p1=5;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,0,1,250,250,0)							
						print_game_name(p1)						
					elseif (state==2) then
						p2=p2+1;
						if (p2>99) then
							p2=99;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,3,4,250,250,0)					
					elseif (state==3) then
						p3=p3+1;
						if (p3>99) then 
							p3=99
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,6,7,250,250,0)					
					end
				end
				-- down key
				if (ck==16) then
					if (state==1) then
						p1=p1-1;
						if (p1<1) then 
							p1=1;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)	
						b:config(r, s,0,1,250,250,0)									
						print_game_name(p1)						
					elseif (state==2) then
						p2=p2-1;
						if (p2<0) then
							p2=0;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,3,4,250,250,0)										
					elseif (state==3) then
						p3=p3-1;
						if (p3<0) then 
							p3=0
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r,s,6,7,250,250,0)					
					end
				end	
			end
		end
		b:render(0)	

		a1:render(0)
		--a2:render(0)
		--a3:render(0)
		game.delay(100000/4);	
		

	end
end


function plot_led (x,y,w,h,c_out,c_in)
	local function plot_fill_rect(x,y,w,h,c)
		for i = 0,w-1,1 do
			game.line (x+i,y,x+i,y+h,c)
		end
	end

	local function plot_rect(x,y,w,h,c)
		game.line(x,y,x+w,y,c)
		game.line(x,y+h-1,x+w,y+h-1,c)
		game.line (x,y,x,y+h-1,c)
		game.line (x+w-1,y,x+w-1,y+h-1,c)	
	end
	-- clean previos rect state
	plot_rect(x,y,w,h,0)	

	-- plot next rect state
	plot_fill_rect(x,y,w,h,c_in)
	plot_rect(x,y,w,h,c_out)
end

function plot_non_active_led (x,y,w,h,c_out)

	local function plot_rect(x,y,w,h,c)
		game.line(x,y,x+w,y,c)
		game.line(x,y+h-1,x+w,y+h-1,c)
		game.line (x,y,x,y+h-1,c)
		game.line (x+w-1,y,x+w-1,y+h-1,c)	
	end
	plot_rect(x,y,w,h,c_out)
	for i = 0,w-1,1 do
		game.plot (x+i,y+i,c_out)
		game.plot (x+i,y+h-1-i,c_out)

	end
--	game.line (x,y,x+w-1,y+h-2,c_out)
	--game.line (x,y+h-1,x+w,y,c_out)
end



function plot_leds()
	local rect_color_out = 0xffffff 
  	local x = 3	
	for i=0,7,1 do
		if (game.is_station_connected(i+1)==1) then 
			if (game.is_station_blink(i+1)==1) then 
				if (game.is_station_blink_on(i+1)==1) then 			
					plot_led(4+i*7,x,6,6,rect_color_out , game.station_get_rgb(i+1) )
				else
					plot_led(4+i*7,x,6,6,rect_color_out , 0 )
				end
			else
				plot_led(4+i*7,x,6,6,rect_color_out , game.station_get_rgb(i+1) )
			end
		else 
			plot_non_active_led(4+i*7,x,6,6,0xffffff  )
		end
	end
end

function play(p1,p2,p3)
		game.plot (x+i,y+i,c_out)
	print (p1,p2,p3)
	plot_fill_rect(10,3,6,6,0xff00ff)	
end



function calc_time(t)game.get_timer()
	 local min = math.floor(t/60);
	 local sec = t - math.floor( (t/60))*60

	local minString =min  < 10 and "0" .. min or tostring(min)
	local secString = sec < 10 and "0" .. sec or tostring(sec)
	return minString .. ":" ..  secString
end

function game1()

end



function game1_led_select(data)
	
	local leds = data['active_leds']
	if #leds > 1
	then
		local blink_id = data['blink_id']
		local dir = data['dir']
		if (dir==1) then
			repeat
				blink_id = blink_id + 1
				if (blink_id == #leds + 1) then
					dir = 0;
					blink_id = blink_id - 2
				end
			until game.is_station_connected(leds[blink_id]) == 1 
		elseif (dir == 0) then
			repeat
				blink_id = blink_id  - 1
				if (blink_id == 0) then
					dir = 1;
					blink_id = 2
				end
			until game.is_station_connected(leds[blink_id]) == 1 

		end
		data['blink_id'] = blink_id
		data['dir'] = dir
	end

end


function game2_led_select(data) 
	math.randomseed(os.time())  -- Initialize the random seed
	local leds = data['active_leds']

	if #leds > 1
	then
		local randomInteger = math.random(1, #leds)  -- Generates a random integer between 1 and 8			
		local blink_id = data['blink_id']
		while blink_id == randomInteger
		do
			randomInteger = math.random(1, #leds) 
		end
		data['blink_id'] = randomInteger
	end
end


function game3_led_select(data)
	
	local leds = data['active_leds']
	if #leds > 1
	then
		local blink_id = data['blink_id']
		repeat
			blink_id = blink_id + 1
			if (blink_id == #leds + 1) then
				blink_id = 1
			end
		until game.is_station_connected(leds[blink_id]) == 1 
		data['blink_id'] = blink_id
	end

end


function game4_led_select(data) 
	local leds = data['active_leds']
	if #leds > 1
	then
		local blink_id 
		if data['state'] == 1
		then
			blink_id =1
			data['state'] = 2
		else
			blink_id = math.random(2, #leds) 
			data['state'] = 1
		end
		data['blink_id'] = blink_id
	end


end

local function active_leds_list()
	local leds = {}
	local i
	local blink_id = 1
	for  i = 1,8,1
	do
		if game.is_station_connected(i) == 1 
		then
			leds[blink_id] = i
			blink_id = blink_id + 1								
		end
		
	end
	return leds
end

function play_all(tries, led_duration, game_id)

	local leds = active_leds_list()

	-- set game 1 cong data
	local game1_data = {}
	game1_data['dir'] = 1
	game1_data['blink_id'] = 1
	game1_data['active_leds'] =leds
	game1_data['select'] = game1_led_select

	-- set game 2 cong data
	local game2_data = {}	
	game2_data['blink_id'] = 1
	game2_data['select'] = game2_led_select
	game2_data['active_leds'] =leds	
	game2_data:select(game2_data)

	-- set game 1 cong data
	local game3_data = {}
	game3_data['blink_id'] = 1
	game3_data['active_leds'] =leds
	game3_data['select'] = game3_led_select


	-- set game 4 cong data
	local game4_data = {}	
	game4_data['blink_id'] = 1
	game4_data['select'] = game4_led_select
	game4_data['active_leds'] =leds		
	game4_data['state'] = 1  -- 1 in base , 2 in some led
	
	-- set game 5 cong data
	local game5_data = {}	
	game5_data['blink_id'] = 1
	game5_data['select'] = game2_led_select
	game5_data['active_leds'] =leds	
	game5_data:select(game5_data)
	

	-- select the config data acoring to game_id
	local game_data = {}
	game_data[1] = game1_data
	game_data[2] = game2_data
	game_data[3] = game3_data
	game_data[4] = game4_data
	game_data[5] = game5_data

	local data =  game_data[game_id]


	local score = 0

	if (game_id == 5)
	then
		-- timer down from tries to 0
		game.set_timer(0,tries * 60)
		num_of_tries = 0
	else
		-- timer up from 0 to inf
		game.set_timer(1,0)
		num_of_tries = 	tries

	end
	game.blink(leds[data['blink_id']],5,led_duration)
	while (((game_id <5 and tries > 0) or (game_id == 5 and game.get_timer()>0))  and (game.keys()&(1<<2))==0 ) 
	do

		if (game.is_station_blink(leds[data['blink_id']]) == 0) then
			if (game.stop_reason( leds[data['blink_id']]) == 1) then
				score = score + 1
			end
			-- call led selction function
			data:select (data)
			
			if (game_id < 5)
			then
				tries = tries - 1			
				if (tries>0) then
					game.blink(leds[data['blink_id']],5,led_duration)
				end
			else
				game.blink(leds[data['blink_id']],5,led_duration)
				num_of_tries = num_of_tries + 1
			end
		end	

		plot_leds()


		print_game_name(game_id)
		game.print (calc_time(game.get_timer()), 1,9,0)
		game.print (string.format("%d/%d",score,num_of_tries), 34,9,0)

		a1:render(0)
		--a2:render(0)	
		game.delay(100000/4);
		
	end
	init()
	local finale_time = game.get_timer()
	while (game.keys() == 0) 
	do	
		game.clean()
		plot_leds()
		print_game_name(1)
		game.print (calc_time(finale_time), 1,9,0)
		game.print (string.format("%d/%d",score,num_of_tries), 34,9,0)
		a1:render(0)
		render(0) -- render the fireworks
		game.delay(100000/4);

	end

end




game.led_rgb(1,255,255,255)
game.led_rgb(2,0  ,255,255)
game.led_rgb(3,255,0,255)
game.led_rgb(4,0  ,0,255)
game.led_rgb(5,255,255,0)
game.led_rgb(6,0  ,255,0)
game.led_rgb(7,255,0,0)
game.led_rgb(8,255  ,0,255)



while (true)
do
	for i= 1,8,1
	do
		local r = 0
		local g = 0 
		local b = 0
		while (r == 0 and b == 0 and g == 0)
		do
			r = math.random(0, 1) == 0 and 255 or 0
			g = math.random(0, 1) == 0 and 255 or 0
			b = math.random(0, 1) == 0 and 255 or 0
		end 
		game.led_rgb(i,r,g,b)		
	end
	game.clean()
	game.opacity(0.7,0.3,0.3)
	game_id,led_on_duration,tries = config()
	game.clean()	
	play_all(tries,led_on_duration*1000,game_id )
	print("game over")
end

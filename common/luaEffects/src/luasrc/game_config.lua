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



function calc_time(t)
	 local min = math.floor(t/60);
	 local sec = t - math.floor( (t/60))*60

	local minString =min  < 10 and "0" .. min or tostring(min)
	local secString = sec < 10 and "0" .. sec or tostring(sec)
	return minString .. ":" ..  secString
end

function game1()

end



function game1_led_select(data) 
	local blink_id = data['blink_id']
	dir = data['dir']
	if (dir==1) then
		repeat
			blink_id = blink_id + 1
			if (blink_id == 9) then
				dir = 0;
				blink_id = 7
			end
		until game.is_station_connected(blink_id) == 1
	elseif (dir == 0) then
		repeat
			blink_id = blink_id  - 1
			if (blink_id == 0) then
				dir = 1;
				blink_id = 2
			end
		until game.is_station_connected(blink_id) == 1

	end
	data['blink_id'] = blink_id
	data['dir'] = dir

end


function game2_led_select(data) 
	math.randomseed(os.time())  -- Initialize the random seed
	local randomInteger = math.random(1, 8)  -- Generates a random integer between 1 and 8
	local leds = {1,2,3,4,5,6,7,8}
		
	blink_id = data['blink_id']
	while blink_id == leds[randomInteger]
	do
		randomInteger = math.random(1, 8) 
	end
	data['blink_id'] = leds[randomInteger]


end


function play_all(tries, led_duration, game_id)

	-- set game 1 cong data
	local game1_data = {}
	game1_data['dir'] = 1
	game1_data['blink_id'] = 1
	game1_data['select'] = game1_led_select

	-- set game 2 cong data
	local game2_data = {}	
	game2_data['blink_id'] = 1
	game2_data['select'] = game2_led_select
	game2_data:select(game2_data)

	-- select the config data acoring to game_id
	local game_data = {}
	game_data[1] = game1_data
	game_data[2] = game2_data
	local data =  game_data[game_id]


	local score = 0
	local num_of_tries = 	tries

	game.clean()	
	game.set_timer(1,0)
	game.blink(data['blink_id'],5,led_duration)
	while (tries > 0) 
	do

		if (game.is_station_blink(data['blink_id']) == 0) then
			if (game.stop_reason( data['blink_id']) == 1) then
				score = score + 1
			end
			-- call led selction function
			data:select (data)

			tries = tries - 1			
			if (tries>0) then
				game.blink(data['blink_id'],5,led_duration)
			end
		end	

		plot_leds()


		print_game_name(1)
		game.print (calc_time(game.get_timer()), 1,9,0)
		game.print (string.format("%d/%d",score,num_of_tries), 34,9,0)

		a1:render(0)
		--a2:render(0)	
		game.delay(100000/4);
		
	end
	game.print (calc_time(game.get_timer()), 1,9,0)
	game.print (string.format("%d/%d",score,num_of_tries), 34,9,0)
	a1:render(0)

end


function game1(tries, led_duration)
	local game1_data = {}

	local blink_id = 1
	local dir=1
	local score = 0

	game.clean()	
	game.set_timer(1,0)
	local num_of_tries = 	tries
	game.blink(blink_id,5,led_duration)
	while (tries > 0) 
	do

		if (game.is_station_blink(blink_id) == 0) then
			if (game.stop_reason( blink_id) == 1) then
				score = score + 1
			end
			
			if (dir==1) then
				blink_id = blink_id + 1
				if (blink_id == 9) then
					dir = 0;
					blink_id = 7
				end
			elseif (dir == 0) then
				blink_id = blink_id  - 1
				if (blink_id == 0) then
					dir = 1;
					blink_id = 2
				end
			end
			tries = tries - 1			
			if (tries>0) then
				game.blink(blink_id,5,led_duration)
			end
		end	

		plot_leds()


		print_game_name(1)
		game.print (calc_time(game.get_timer()), 1,9,0)
		game.print (string.format("%d/%d",score,num_of_tries), 34,9,0)

		a1:render(0)
		--a2:render(0)	
		game.delay(100000/4);
		
	end
	game.print (calc_time(game.get_timer()), 1,9,0)
	game.print (string.format("%d/%d",score,num_of_tries), 34,9,0)
	a1:render(0)

end


game.led_rgb(1,255,255,255)
game.led_rgb(2,0  ,255,255)
game.led_rgb(3,255,0,255)
game.led_rgb(4,0  ,0,255)
game.led_rgb(5,255,255,0)
game.led_rgb(6,0  ,255,0)
game.led_rgb(7,255,0,0)
game.led_rgb(8,255  ,0,255)


game.clean()
game.opacity(0.7,0.3,0.3)
game_id,led_on_duration,tries = config()
--game1(tries,led_on_duration*1000)

play_all(tries,led_on_duration*1000,game_id )
print("game over")


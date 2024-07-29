--require("t3")
--
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
	a1=frame.new(rect.new(0,0,64,32),0xff00ff,0x00ff00, 5,1 ,1);
	a2=frame.new(rect.new(1,1,62,30),0xff0000,0x0000ff, 10,1 ,-5);
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
		a2:render(0)
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
	local rect_color_out = 0xff00ff 
	
	for i=0,7,1 do
		if (game.is_station_connected(i+1)==1) then 
			if (game.is_station_blink(i+1)==1) then 
				if (game.is_station_blink_on(i+1)==1) then 			
					plot_led(4+i*7,5,6,6,rect_color_out , game.station_get_rgb(i+1) )
				else
					plot_led(4+i*7,5,6,6,rect_color_out , 0 )
				end
			else
				plot_led(4+i*7,5,6,6,rect_color_out , game.station_get_rgb(i+1) )
			end
		else 
			plot_non_active_led(4+i*7,5,6,6,0xffffff  )
		end
	end
end

function play(p1,p2,p3)
		game.plot (x+i,y+i,c_out)
	print (p1,p2,p3)
	plot_fill_rect(10,3,6,6,0xff00ff)	
end


game.clean()
game.opacity(0.7,0.3,0.3)
p1,p2,p3 = config()
game.clean()
--rect_color_out = 0xff00ff 
--rect_color_in = 0x0000ff 

plot_leds()


game.led_rgb(2,0,240,0)
game.blink(2,2,60000)

p1=1

while (1) 
do
	game.clean()	
	print_game_name(p1)	
	plot_leds()	
	game.delay(100000/4);	
end


play(1,2,3)

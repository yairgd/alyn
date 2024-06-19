--require("t3")
function config()
	a1=frame.new(rect.new(0,0,64,32),0xff00ff,0xfff00, 5,1 ,1);
	a2=frame.new(rect.new(1,1,62,30),0x000fff,0x0ffff, 10,1 ,1);

	game.clean()
	p1=1
	p2=2;
	p3=3;
	m1="העדוה"
	m2="רטמרפ"
	m3="msg3"
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
	b=banner2.new(rect.new(8,4,62,15), s,0,1,0.25,0,0)
	mb=banner2.new(r2, "       ",1,0,0.25,0,0)

	-- state: 0 - no blink, 1 (fro mleft)  blink, 2 2 blink 3 blinks 

	while( 1 )
	do
		k = game.keys()
		lk = game.long_keys()

				
		for i=0,4,1
		do
			ck = k&(1<<i);
			lck = lk&(1<<i);

			if (ck>0 or lck>0) then
				--print (ck,state,i)
				game.set_key(i,0);
				game.set_long_key(i,0);
				if (ck == 4 and state ==0) then
					return p1,p2,p3
				end
			
				if (lck == 1 and state==0) then
						-- begin to blink
						mb:config(r2, m1,0,0,0,0,0)	
						state=1
						b:config(r, s,0,1,0.25,0.25,0)				
				elseif (ck==1 and state >0 ) then
						-- no blink
						state=0;
						mb:config(r2, "     ",0,1,0.25,0.25,0)
						b:config(r, s,0,1,0.25,0,0)				
				end

				if (ck == 8) then
					-- move blink to right
					if (state==1) then
						b:config(r, s,  3,4,0.25,0.25,0)
						mb:config(r2, m2,0,0,0,0,0)
						state=2;
					elseif (state==2) then
						b:config(r, s,6,7,0.25,0.25,0)
						mb:config(r2, m3,0,0,0,0,0)
						state=3;
					elseif (state==3) then
						b:config(r, s,0,1,0.25,0.25,0)
						mb:config(r2, m1,0,0,0,0,0)
						state=1;
					end
				end
				if (ck == 4) then
					-- move blink to left
					if (state==3) then
						b:config(r, s,3,4,0.25,0.25,0)
						mb:config(r2, m2,0,0,0,0,0)
						state=2;
					elseif (state==2) then
						b:config(r, s,0,1,0.25,0.25,0)
						mb:config(r2, m1,0,0,0,0,0)
						state=1;
					elseif (state==1) then
						b:config(r, s,6,7,0.25,0.25,0)
						mb:config(r2, m3,0,0,0,0,0)
						state=3;
					end
				end
				-- up key
				if (ck==2) then
					if (state==1) then
						p1=p1+1;
						if (p1>99) then 
							p1=99;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,0,1,0.25,0.25,0)							
					elseif (state==2) then
						p2=p2+1;
						if (p2>99) then
							p2=99;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,3,4,0.25,0.25,0)					
					elseif (state==3) then
						p3=p3+1;
						if (p3>99) then 
							p3=99
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,6,7,0.25,0.25,0)					
					end
				end
				-- down key
				if (ck==16) then
					if (state==1) then
						p1=p1-1;
						if (p1<0) then 
							p1=0;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)	
						b:config(r, s,0,1,0.25,0.25,0)									
					elseif (state==2) then
						p2=p2-1;
						if (p2<0) then
							p2=0;
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r, s,3,4,0.25,0.25,0)										
					elseif (state==3) then
						p3=p3-1;
						if (p3<0) then 
							p3=0
						end
						s = string.format("%2d:%2d:%2d",p1,p2,p3)
						b:config(r,s,6,7,0.25,0.25,0)					
					end
				end	
			end
		end
		b:render(0)	
		mb:render(0)	

		a1:render(1)
		a2:render(1)
		game.delay(100000/4);	

	end
end

game.clean()
game.opacity(0.7,0.3,0.3)
config()

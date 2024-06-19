--a1=frame.new(rect.new(0,0,64,32),0xff00ff,0xfff00, 5,1 ,1);

r2 = rect.new(18,17,62,15);
mb=banner2.new(r2, " test   ",1,0,0.25,0,0)

for i = 0,32,1 
do 
  -- a1:render(2)
  mb:render(0)
   print(i) 	
   game.delay(1000000);
   game.plot (i,i,255*255*255);
end

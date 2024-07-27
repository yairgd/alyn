
game.clean()
j=0;
k=0;
for i = 5512,0,-1 
do 

	print(i)
	game.set_local_rect(rect.new(k,k,30,10),rect.new(k,k,200,10)) 	
	game.print("moving text - תוזז תויתוא", i,0,2)
	game.set_global_rect()

	game.delay(100000/4);
	game.plot (i,i,255*255*255);

	j=j+1;
	j= j%10
	if (j==0)
	then
		k=k+1
	end
	k= k %32
	game.clean()

		
end


function wait(seconds)
    local start = os.time() 
    while os.time() - start < seconds do 
    end 
end 
game.banner_print("אבגדהabcd")
for i = 10,1,-1 
do 
   game.enable(i % 8 + 1)
   game.delay(1000000);
   print ("test ",i)
end 

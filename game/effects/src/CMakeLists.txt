set(Effects_SRC
	fonts/6x13-ISO8859-8.c  
	fonts/9x18-ISO8859-8.c  
	banner.c  
	canvas.c  
	effect.c
	font.c  
	frame.c  
	led_matrix.c  
	u8.c
	lua/frame_effect.c
	lua/banner_effect_1.c
	lua/banner_effect_2.c
	lua/rect.c
	lua/obj.c
	lua/lgamelib.c
)




add_library(effects_static STATIC
	${Effects_SRC}
)
target_link_libraries(effects_static 
	lua
	common_static
)

target_include_directories(effects_static PUBLIC 
	.
	${PROJ_TOP}/game/lua-5.4.6/src
)






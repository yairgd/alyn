set(Effects_SRC
	6x13-ISO8859-8.c  
	9x18-ISO8859-8.c  
	banner.c  
	canvas.c  
	effect.c
	font.c  
	frame.c  
	led_matrix.c  
	u8.c
	frame_lua_effect.c
)

add_library(effects_static STATIC
	${Effects_SRC}
)

target_include_directories(effects_static PUBLIC 
	.
	${CMAKE_SOURCE_DIR}/game/lua-5.4.6/src
)


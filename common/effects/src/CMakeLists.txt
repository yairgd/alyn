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
)
add_library(effects_static STATIC
	${Effects_SRC}
)


if (${ZEPHYR})
	target_compile_definitions(effects_static PRIVATE -DZEPHYR)
endif()

target_link_libraries(effects_static 
	lua_module
	common_static
)

target_include_directories(effects_static PUBLIC 
	.
	./src
	${PROJ_TOP}/game/lua-5.4.6/src
	${PROJ_TOP}/game/common	
)



target_compile_options(effects_static PRIVATE ${cpu_compile_flags})


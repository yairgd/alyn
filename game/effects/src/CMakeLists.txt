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
	timing.c
	lua/frame_effect.c
	lua/banner_effect_1.c
	lua/banner_effect_2.c
	lua/rect.c
	lua/obj.c
	lua/lgamelib.c

)

if (NOT ${ZEPHYR})
	list(APPEND Effects_SRC "timing.c")
endif()

add_library(effects_static STATIC
	${Effects_SRC}
)
target_link_libraries(effects_static 
	lua
)

target_include_directories(effects_static PUBLIC 
	.
	${CMAKE_SOURCE_DIR}/game/lua-5.4.6/src
)


# decide which lua files to embbed in the code
set(lua_source_files
	${CMAKE_CURRENT_SOURCE_DIR}/../lua/bouncing_balls.lua
	${CMAKE_CURRENT_SOURCE_DIR}/../lua/demo1.lua
)
lua_generate_c_files_for_lua_binary_target(${lua_source_files})	


# target to create binary library to hold all bibary data - 
# it also trigers the generation of the c files
add_library(lua_binaries 
	${generated_c_files}
	${CMAKE_CURRENT_BINARY_DIR}/joint_file.c
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc.c
)
target_include_directories (lua_binaries PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

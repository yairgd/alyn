


# decide which lua files to embbed in the code
set(lua_source_files
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/bouncing_balls.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/simple_fireworks.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/scrolling_color.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/demo1.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/loop.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/demo2.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/game_config.lua
	${CMAKE_CURRENT_SOURCE_DIR}/luasrc/demo3.lua

)

make_lua_binaries(${lua_source_files})

set(luaEffects_SRC
	banner_effect_1.c  
	banner_effect_2.c  
	frame_effect.c  
	lgamelib.c  
	obj.c  
	rect.c
)


add_library(luaEffects_static STATIC
	${luaEffects_SRC}
)

target_link_libraries(luaEffects_static
	lua_module
	lua
	effects_static
)

target_include_directories (luaEffects_static PUBLIC
	.
	..
)

target_compile_options(luaEffects_static PRIVATE ${cpu_compile_flags})


cmake_minimum_required(VERSION 3.10.0)

project(luac)

# https://github.com/zephyrproject-rtos/zephyr/discussions/45485
# Get a list of all source files in the current directory
set(LUA_SOURCES
	src/lapi.c
	src/lauxlib.c
	src/lbaselib.c
	src/lcode.c
	src/luac.c
	src/lgc.c
	src/lcorolib.c
	src/lctype.c
	src/ldebug.c
	src/ldo.c
	src/ldump.c
	src/lfunc.c
	src/lgc.c
	src/liolib.c
	src/llex.c
	src/lmathlib.c
	src/lmem.c
	src/lobject.c
	src/lopcodes.c
	src/lparser.c
	src/lstate.c
	src/lstring.c
	src/lstrlib.c
	src/ltable.c
	src/ltablib.c
	src/ltm.c
	src/lundump.c
	src/lvm.c
	src/lzio.c
	src/linit.c
	src/ldblib.c
	src/loslib.c
	src/loadlib.c
	#  src/lbitlib.c
)

if (NOT ${TARGET} STREQUAL "ZEPHYR_STM32")
		set (mem ./memory.c)
endif()

# Lua librarary to integrate in the MCU code
add_library(lua_module 
	${LUA_SOURCES} 
	${mem}
)

#	target_compile_definitions(lua_module PRIVATE
#		-D CONFIG_LUA_GAME_LIB
#		-D LUA_USE_READLINE
#	)



# Include directories for your subdirectory
target_include_directories(lua_module PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	../
)
target_compile_options(lua_module PRIVATE ${cpu_compile_flags})

# Link any necessary dependencies
target_link_libraries(lua_module
	common_static
	)


#set (vv $<IF:$<STREQUAL:${CMAKE_CXX_COMPILER_ID},"GNU">,"-DLUA_USE_READLINE","">)
target_compile_definitions (lua_module PRIVATE
	-D CONFIG_LUA_GAME_LIB
	#	-DLUA_USE_READLINE
	#-D $<IF:$<STREQUAL:${CMAKE_CXX_COMPILER_ID},"GNU">,"LUA_USE_READLINE","">	
)





# build the lua standalone module to run in the GUI as separate thread
add_library(lua 
	src/lua.c
	${mem}
	${LUA_SOURCES}
)

target_link_libraries(lua
	${linux_lib}	
	common_static
)
target_compile_options(lua PRIVATE ${cpu_compile_flags})

if (${USE_GUI_CONSOLE})
	target_compile_definitions(lua PRIVATE
		-D CONFIG_LUA_GAME_LIB
		-D LUA_USE_READLINE
	)
else()
	target_compile_definitions(lua PRIVATE
		-D CONFIG_LUA_GAME_LIB
		-D USE_ORIGNAL_LUA_CODE
		${msvc_readline}
	)
endif()






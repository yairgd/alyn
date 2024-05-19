# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

set (PROJ_TOP "/home/yair/alyn/project/")


add_compile_definitions( 
	ZEPHYR 
	USE_ORIGNAL_LUA_CODE
	NO_SYSLOG
)

set (ZEPHYR  TRUE)
set (cpu_compile_flags -mcpu=cortex-m4 -mthumb)

add_subdirectory(${PROJ_TOP}/game/lua-5.4.6/  ${PROJECT_BINARY_DIR}/lua-5.4.6/)
add_subdirectory(${PROJ_TOP}/game/effects  ${PROJECT_BINARY_DIR}/effects)
add_subdirectory(${PROJ_TOP}/game/common  ${PROJECT_BINARY_DIR}/common)


#https://jonathanhamberg.com/post/cmake-file-embedding/
include (${PROJ_TOP}/cmake/functions.cmake)
set(PRE_CONFIGURE_FILE "${PROJ_TOP}/cmake/git.h.in")
set(POST_CONFIGURE_FILE "${PROJECT_BINARY_DIR}/git.h")
include (${PROJ_TOP}/cmake/git_watcher.cmake)
set (CMAKE_CXX_STANDARD 17)

add_library(common_static1 STATIC IMPORTED GLOBAL)
set_target_properties(common_static1 PROPERTIES IMPORTED_LOCATION
	/home/yair/alyn/project/game-controller/build/common/libcommon_static.a)

target_sources(app PRIVATE src/main.c
	src/shell.c
	src/main.c	
	src/hwctl.c	
	src/game.c
  	src/uart.c
  	src/display.c

	src/timing.c

  	src/parser_task.cpp

	#	common/system_model.c
	#common/logger.c


	${PROJ_TOP}/game/comm_protocol/protocol-v1/ProtocolStateMachine.cpp
	${PROJ_TOP}/game-controller/src/HandleUartMsg.cpp
	${PROJ_TOP}/game/hal/zephyr/uart_shell.cpp

	#	src/getopt/getopt.c
)

get_target_property(TARGET_COMPILE_OPTIONS app COMPILE_OPTIONS)
message("Compile options for my_target: ${TARGET_COMPILE_OPTIONS}")
target_include_directories(app PRIVATE
	${PROJ_TOP}/game/comm_protocol/
	${PROJ_TOP}/game/
	${PROJ_TOP}/game/hal
	${PROJ_TOP}/
	${PROJECT_BINARY_DIR}
	src/getopt
	common/	
	)


target_link_libraries(app PRIVATE 
	-Wl,--start-group	
	effects_static
		common_static
	lua_binaries
	lua_module
	#lua
	-Wl,--end-group

)
add_dependencies( app
	common_static1
)
#target_link_libraries(app PRIVATE common_static)

target_compile_definitions(app PRIVATE 
	USE_ORIGNAL_LUA_CODE
	NO_SYSLOG)


#ua_generate_inc_file_for_target(
#	app
#	${CMAKE_CURRENT_SOURCE_DIR}/lua/game1.lua
#)


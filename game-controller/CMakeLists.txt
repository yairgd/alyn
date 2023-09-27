# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

set (PROJ_TOP "/home/yair/alyn/project/")

add_subdirectory(libs/lua)


include (${PROJ_TOP}/cmake/functions.cmake)
set(PRE_CONFIGURE_FILE "${PROJ_TOP}/cmake/git.h.in")
set(POST_CONFIGURE_FILE "${PROJECT_BINARY_DIR}/git.h")
include (${PROJ_TOP}/cmake/git_watcher.cmake)
set (CMAKE_CXX_STANDARD 17)

target_sources(app PRIVATE src/main.c
	src/shell.c
	src/system_model.c
	src/main.c	
	src/hwctl.c	
	src/game.c
  	src/lgamelib.c
  	src/uart.c
  	src/u8.c
  	src/font.c
  	src/banner.c


  	src/parser_task.cpp

	${PROJ_TOP}/game/comm_protocol/protocol-v1/ProtocolStateMachine.cpp
	${PROJ_TOP}/game-controller/src/HandleUartMsg.cpp
	${PROJ_TOP}/game/hal/zephyr/uart_shell.cpp

	#	src/getopt/getopt.c
)


target_include_directories(app PRIVATE
	${PROJ_TOP}/game/comm_protocol/
	${PROJ_TOP}/game/
	${PROJ_TOP}/game/hal
	${PROJ_TOP}/
	${PROJECT_BINARY_DIR}
	)

target_include_directories(app PRIVATE
	src/getopt
)
target_link_libraries(app PRIVATE lua_module)

target_compile_definitions(app PUBLIC
	-D NO_SYSLOG)


lua_generate_inc_file_for_target(
	app
	${CMAKE_CURRENT_SOURCE_DIR}/lua/game1.lua
)


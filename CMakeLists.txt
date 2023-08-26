# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)


add_subdirectory(libs/lua)


target_sources(app PRIVATE src/main.c
	src/shell.c
	src/system_model.c
	src/main.c	
	src/hwctl.c	
	src/game.c
  	src/lgamelib.c
	#	src/getopt/getopt.c
)

target_include_directories(app PRIVATE
	src/getopt
)
target_link_libraries(app PRIVATE lua_module)




lua_generate_inc_file_for_target(
	app
	${CMAKE_CURRENT_SOURCE_DIR}/lua/game1.lua
)


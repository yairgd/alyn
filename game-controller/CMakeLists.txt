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


set(TARGET  ZEPHYR)


include(ExternalProject)

# Define the external project
ExternalProject_Add(luac
    SOURCE_DIR ${PROJ_TOP}/common/luaEffects/luac
    #  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    BUILD_ALWAYS 1
    INSTALL_COMMAND "" 
    PREFIX ${CMAKE_BINARY_DIR}/external_project_build 
)


# Make sure the main project depends on the custom target
add_dependencies(app luac)


set (LUAC_PATH 	
		${CMAKE_BINARY_DIR}/external_project_build/src/luac-build/luac
)

set (ZEPHYR  TRUE)
set (cpu_compile_flags -mcpu=cortex-m4 -mthumb)


add_subdirectory(${PROJ_TOP}/common/luaEffects  ${PROJECT_BINARY_DIR}/luaEffects/)
add_subdirectory(${PROJ_TOP}/common/effects  ${PROJECT_BINARY_DIR}/effects)
add_subdirectory(${PROJ_TOP}/common/utils  ${PROJECT_BINARY_DIR}/utils)
#add_subdirectory(${PROJ_TOP}/common/comm_protocol  ${PROJECT_BINARY_DIR}/comm_protocol)
#add_subdirectory(${PROJ_TOP}/common/hal  ${PROJECT_BINARY_DIR}/hal)


#https://jonathanhamberg.com/post/cmake-file-embedding/
include (${PROJ_TOP}/cmake/functions.cmake)
set(PRE_CONFIGURE_FILE "${PROJ_TOP}/cmake/git.h.in")
set(POST_CONFIGURE_FILE "${PROJECT_BINARY_DIR}/git.h")
include (${PROJ_TOP}/cmake/git_watcher.cmake)
set (CMAKE_CXX_STANDARD 17)

#add_library(common_static1 STATIC IMPORTED GLOBAL)
#set_target_properties(common_static1 PROPERTIES IMPORTED_LOCATION
#	/home/yair/alyn/project/game-controller/build/common/libcommon_static.a)

target_sources(app PRIVATE 
	# c
	src/shell.c
	src/main.c	
	src/hwctl.c	
	src/game.c
  	src/uart.c
	src/display.c
	src/buttons.c
	src/timing.c
  	src/lua_memory.c

	# cpp	
  	src/parser_task.cpp
	src/HandleUartMsg.cpp
	${PROJ_TOP}/common/comm_protocol/protocol-v1/ProtocolStateMachine.cpp
	${PROJ_TOP}/common/hal/zephyr/uart_shell.cpp

)

get_target_property(TARGET_COMPILE_OPTIONS app COMPILE_OPTIONS)
message("Compile options for my_target: ${TARGET_COMPILE_OPTIONS}")
target_include_directories(app PRIVATE
	${PROJ_TOP}/common/comm_protocol/
	${PROJ_TOP}/common/
	${PROJ_TOP}/common/hal
	${PROJ_TOP}/
	${PROJECT_BINARY_DIR}
	src/getopt
	common/	
	)


target_link_libraries(app PRIVATE 
	-Wl,--start-group	
	effects_static
	common_static
	luaEffects_static
	lua_binaries
	lua_module
	#	comm_protocol
	#	Hal
	-Wl,--end-group

)
#add_dependencies( app
#	common_static1
#)

#target_link_libraries(app PRIVATE common_static)

target_compile_definitions(app PRIVATE 
	ZEPHYR
	USE_ORIGNAL_LUA_CODE
	NO_SYSLOG)




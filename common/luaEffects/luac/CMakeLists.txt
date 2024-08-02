cmake_minimum_required(VERSION 3.20.0)

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
	src/luac.c
	#  src/lbitlib.c
)

set(NEW_LUA_SOURCES "")
foreach(source ${LUA_SOURCES})
    list(APPEND NEW_LUA_SOURCES "../lua-5.4.6/${source}")
endforeach()


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set (msvc_readline "-D LUA_USE_READLINE")
endif()

add_executable(luac
	${NEW_LUA_SOURCES}
	../lua-5.4.6/memory.c
)


target_link_libraries(luac
	m
	${linux_lib}
	
)
target_include_directories (luac PRIVATE
	.
	)
	


target_compile_definitions(luac PRIVATE
	-D USE_ORIGNAL_LUA_CODE
)






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



# Add a library for your subdirectory
add_library(lua_module ${LUA_SOURCES})

# Include directories for your subdirectory
target_include_directories(lua_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	../
)

# Link any necessary dependencies
#target_link_libraries(lua_module
	#	zephyr)

target_compile_definitions (lua_module PRIVATE -D ZEPHYR)


set(
	LUAC_PATH 
	${CMAKE_CURRENT_BINARY_DIR}/luac  
	CACHE INTERNAL "" )

# LIST( APPEND CMAKE_PROGRAM_PATH  ${LUAC_PATH} )

add_custom_command( OUTPUT ${LUAC_PATH}
	COMMAND make linux
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(  luac
	DEPENDS ${LUAC_PATH}
)


# Add a library for your subdirectory
add_library(lua 
	  src/lua.c
	)
target_link_libraries(lua 
	lua_module
	m)
target_compile_definitions(lua PRIVATE
	-DRUN_AS_LIBRARY
)




function( lua_generate_inc_file_for_target target srcPath )
	get_filename_component( srcName ${srcPath} NAME)
	set(destPath ${CMAKE_CURRENT_BINARY_DIR}/lua/${srcName}.o)
	add_custom_command(
		OUTPUT ${destPath}
		COMMAND ${LUAC_PATH} -o ${destPath} -- ${srcPath}
		DEPENDS luac ${srcPath}
	)
	generate_inc_file_for_target(
		${target}
		${destPath}
		${PROJECT_BINARY_DIR}/zephyr/include/generated/${srcName}.h
	)
endfunction()



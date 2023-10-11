# https://github.com/zephyrproject-rtos/zephyr/discussions/45485
# Get a list of all source files in the current directory
set(LUA_SOURCES
  lapi.c
  lauxlib.c
  lbaselib.c
  lcode.c
  luac.c
  lgc.c
  lcorolib.c
  lctype.c
  ldebug.c
  ldo.c
  ldump.c
  lfunc.c
  lgc.c
  liolib.c
  llex.c
  lmathlib.c
  lmem.c
  lobject.c
  lopcodes.c
  lparser.c
  lstate.c
  lstring.c
  lstrlib.c
  ltable.c
  ltablib.c
  ltm.c
  lundump.c
  lvm.c
  lzio.c
  linit.c
  ldblib.c
  #  loslib.c
  #lua.c
)

# Add a library for your subdirectory
add_library(lua_module ${LUA_SOURCES})

# Include directories for your subdirectory
target_include_directories(lua_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	../
)

# Link any necessary dependencies
target_link_libraries(lua_module zephyr)

target_compile_definitions (lua_module PRIVATE -DZEPHYR)


set(
	LUAC_PATH 
	${CMAKE_CURRENT_SOURCE_DIR}/luac  
	CACHE INTERNAL "" )

# LIST( APPEND CMAKE_PROGRAM_PATH  ${LUAC_PATH} )

add_custom_command( OUTPUT ${LUAC_PATH}
	COMMAND make all
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(  luac
	DEPENDS ${LUAC_PATH}
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




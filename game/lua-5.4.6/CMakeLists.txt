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


# Lua librarary to integrate in the MCU code
add_library(lua_module ${LUA_SOURCES} )

# Include directories for your subdirectory
target_include_directories(lua_module PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/src
	../
)

# Link any necessary dependencies
#target_link_libraries(lua_module
#	zephyr)

target_compile_definitions (lua_module PRIVATE
	-D CONFIG_LUA_GAME_LIB
)


set(
	LUAC_PATH 
	${CMAKE_CURRENT_BINARY_DIR}/Debug/luac 
	CACHE INTERNAL "" )

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	set (linux_lib "m")
	set(
		LUAC_PATH 
		${CMAKE_CURRENT_BINARY_DIR}/luac 
		CACHE INTERNAL "" )
endif()




# build the lua standalone module to run in the GUI as separate thread
add_library(lua 
	src/lua.c)

target_link_libraries(lua
	lua_module
#	m
)
target_compile_definitions(lua PRIVATE
	-DCONFIG_LUA_GAME_LIB
)


add_executable(luac
	src/luac.c
	)


target_link_libraries(luac
	lua_module
	${linux_lib}
)
	



set (Python3_EXECUTABLE "C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Python37_64\\python")
#set (PYTHON "C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Python37_64\\python")


macro( lua_generate_c_files_for_lua_binary_target  ${lua_source_files} )

	#set (this_file_location ${CMAKE_CURRENT_SOURCE_DIR}/../)
	set (this_file_location ${CMAKE_CURRENT_SOURCE_DIR}/)
	# create targets as the name of the lua files
	foreach(full_path_filename    ${lua_source_files})
		message( ${full_path_filename})
		
		# get the base file name
		get_filename_component(file_name ${full_path_filename} NAME)

		# and replace the path to destantion path
		set (file_name ${CMAKE_CURRENT_BINARY_DIR}/${file_name})

		add_custom_command(
			OUTPUT ${file_name}.o
			COMMAND ${LUAC_PATH} -o  ${file_name}.o -- ${full_path_filename}
			DEPENDS luac ${full_path_filename}
		)

		add_custom_command(
			OUTPUT  ${file_name}.c
			COMMAND  ${Python3_EXECUTABLE} ${this_file_location}/lua_fw_to_c.py script_fie ${file_name}.o 
			DEPENDS ${file_name}.o

		)
	endforeach(full_path_filename   ${lua_source_files})


	# prepare a list of all file files that were generated at lua_generate_inc_file_for_target 
	set (generated_c_files)
	foreach(lua_source_file ${lua_source_files})
		get_filename_component(lua_c_file ${lua_source_file} NAME)
		list(APPEND generated_c_files ${CMAKE_CURRENT_BINARY_DIR}/${lua_c_file}.c)
	endforeach()



	#list(TRANSFORM generated_c_files PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
	add_custom_command(
		OUTPUT   ${CMAKE_CURRENT_BINARY_DIR}/joint_file.c
		COMMAND  ${Python3_EXECUTABLE} ${this_file_location}/lua_fw_to_c.py joint_file ${generated_c_files}
	)



endmacro()


macro (make_lua_binaries)
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
endmacro()




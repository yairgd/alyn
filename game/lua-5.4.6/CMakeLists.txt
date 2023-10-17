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
add_library(lua_module ${LUA_SOURCES})

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
	${CMAKE_CURRENT_BINARY_DIR}/luac  
	CACHE INTERNAL "" )

# LIST( APPEND CMAKE_PROGRAM_PATH  ${LUAC_PATH} )

add_custom_command( 
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/luac  
	COMMAND make linux; 
	COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_SOURCE_DIR}/src/luac ${CMAKE_CURRENT_BINARY_DIR}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target( luac
	DEPENDS ${LUAC_PATH}
)


# build the lua standalone module to run in the GUI as separate thread
add_library(lua 
	src/lua.c
)
target_link_libraries(lua
	lua_module
	m
)
target_compile_definitions(lua PRIVATE
	-DCONFIG_LUA_GAME_LIB
)



function( lua_generate_inc_file_for_target1111  srcPath  )

	get_filename_component(file_name ${srcPath} NAME)

	set(destPath ${file_name}.o)	
	add_custom_command(
		OUTPUT ${destPath}
		COMMAND ${LUAC_PATH} -o  ${CMAKE_CURRENT_BINARY_DIR}/${destPath} -- ${srcPath}
		#DEPENDS luac ${srcPath}
	)

	#set (lua_c_file   ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.c)	
	add_custom_command(
		OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/${file_name}.c
		COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/lua_fw_to_c.py script_fie
		${destPath} 
		DEPENDS ${destPath}
	)

	#list(APPEND generated_c_files  ${lua_c_file} )
	#	string(REGEX REPLACE "/" "_" header_target ${srcPath})
	#add_custom_target(  ${header_target}  
	#	DEPENDS   ${lua_c_file} 
	#)
	#set_property(
	#	TARGET ${header_target}
	#	APPEND  
	#	PROPERTY ADDITIONAL_CLEAN_FILES  "${lua_c_file};${destPath}"
	#)
	#	add_dependencies (lua_module ${header_target} )
	#add_dependencies(ALL ${header_target})
endfunction()




function( lua_generate_inc_file_for_target  full_path_filename  )

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
		COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lua_fw_to_c.py script_fie ${file_name}.o 
		DEPENDS ${file_name}.o

	)

	#list(APPEND generated_c_files  ${lua_c_file} )
	#	string(REGEX REPLACE "/" "_" header_target ${srcPath})
	#add_custom_target(  ${header_target}  
	#	DEPENDS   ${lua_c_file} 
	#)
	#set_property(
	#	TARGET ${header_target}
	#	APPEND  
	#	PROPERTY ADDITIONAL_CLEAN_FILES  "${lua_c_file};${destPath}"
	#)
	#	add_dependencies (lua_module ${header_target} )
	#add_dependencies(ALL ${header_target})
endfunction()




# decide which lua files to embbed in the code
set(lua_source_files
	${CMAKE_CURRENT_SOURCE_DIR}/lua/demo1.lua
	${CMAKE_CURRENT_SOURCE_DIR}/lua/t2.lua
)


# generate c file holding binary data of compiled lua script 
foreach(lua_source_file   ${lua_source_files})
	lua_generate_inc_file_for_target(${lua_source_file})	
endforeach(lua_source_file   ${lua_source_files})





# prepare a list of all file files that were generated above
set (generated_c_files)
foreach(lua_source_file ${lua_source_files})
    get_filename_component(lua_c_file ${lua_source_file} NAME)
    list(APPEND generated_c_files ${CMAKE_CURRENT_BINARY_DIR}/${lua_c_file}.c)
endforeach()



#list(TRANSFORM generated_c_files PREPEND "${CMAKE_CURRENT_BINARY_DIR}/")
add_custom_command(
	OUTPUT   ${CMAKE_CURRENT_BINARY_DIR}/joint_file.c
	COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/lua_fw_to_c.py joint_file ${generated_c_files}
	#DEPENDS ${generated_c_files}
)



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


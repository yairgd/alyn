
set (this_file_location ${PROJ_TOP}/game/luaEffects)
#find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

set (Python3_EXECUTABLE python)
# Check if Python3_EXECUTABLE is found
#if (NOT Python3_EXECUTABLE)
#    message(FATAL_ERROR "Python3 executable not found. Please install Python 3.")
#endif()

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
			${CMAKE_CURRENT_SOURCE_DIR}/src
		)
		target_compile_options(lua_binaries PRIVATE ${cpu_compile_flags})		
endmacro()


macro( lua_generate_c_files_for_lua_binary_target  ${lua_source_files} )
	#get_target_property(LUAC_PATH  RUNTIME_OUTPUT_DIRECTORY)
	        message(STATUS "Generating C files from Lua sources: ${lua_source_files}")
	#set (this_file_location ${CMAKE_CURRENT_SOURCE_DIR}/../)
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
			DEPENDS ${LUAC_PATH} ${full_path_filename}
		)

		add_custom_command(
			OUTPUT  ${file_name}.c lua_fw.h
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
		COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/joint_file.c
		COMMAND  ${Python3_EXECUTABLE} ${this_file_location}/lua_fw_to_c.py joint_file ${generated_c_files}
	)
endmacro()

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/lua-5.4.6)
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/src)


#add_subdirectory(test)
add_subdirectory(src)



macro (make_lua_binaries)
		
		lua_generate_c_files_for_lua_binary_target(${lua_source_files})	


		# target to create binary library to hold all bibary data - 
		# it also trigers the generation of the c files
		add_library(lua_binaries 
			${generated_c_files}
			${CMAKE_CURRENT_BINARY_DIR}/joint_file.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/luasrc.c
		)
		target_include_directories (lua_binaries PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}
				${CMAKE_CURRENT_SOURCE_DIR}/src
		)
endmacro()


# decide which lua files to embbed in the code
set(lua_source_files
	${CMAKE_CURRENT_SOURCE_DIR}/lua/bouncing_balls.lua
	${CMAKE_CURRENT_SOURCE_DIR}/lua/demo1.lua
	${CMAKE_CURRENT_SOURCE_DIR}/lua/loop.lua

)

make_lua_binaries(${lua_source_files})

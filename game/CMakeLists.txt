add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/hal )
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/comm_protocol )
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/effects )
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/lua-5.4.6 )
add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/led_matrix_simulator )

if (FALSE)
message(INFO "File list: ${file_list}")
message("!!!!!!!!!!!!!!!!!!!!!!! " DEBUG)

set(file_list)
set(lua_source_files
    ${CMAKE_CURRENT_SOURCE_DIR}/effects/lua/demo1.lua
    ${CMAKE_CURRENT_SOURCE_DIR}/effects/lua/t2.lua
)

foreach(lua_source_file ${lua_source_files}) 

	lua_generate_inc_file_for_target(${lua_source_file} c_file)	
	string(REGEX REPLACE "/" "_" lua_source_file ${lua_source_file})
	list(APPEND file_list ${c_file})

	#target_sources(lua_binary_code PRIVATE ${c_file})
	
endforeach()
message(INFO "File list: ${file_list}")
endif()
#add_library(lua_binary_code STATIC
# ${file_list}
#)

#lua_generate_inc_file_for_target(
#	${CMAKE_CURRENT_SOURCE_DIR}/effects/lua/demo1.lua
#)
#add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/tests )





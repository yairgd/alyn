

add_executable(test1 
	${CMAKE_CURRENT_SOURCE_DIR}/test1.cpp
)

target_include_directories(test1 PRIVATE 
	${PROJECT_BINARY_DIR}/	
	${CMAKE_SOURCE_DIR}	
	${CMAKE_CURRENT_SOURCE_DIR}	
	..
)

target_link_libraries(test1 PRIVATE
	Hal
	protocol_static
	$<$<CXX_COMPILER_ID:MSVC>:>$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:pthread>
	common_static
)



add_executable(load-lua 
	${CMAKE_CURRENT_SOURCE_DIR}/load_lua.cpp

	
)

target_include_directories(load-lua PRIVATE 
	${PROJECT_BINARY_DIR}/	
	${CMAKE_SOURCE_DIR}	
	${CMAKE_CURRENT_SOURCE_DIR}	
	..
)

target_link_libraries(load-lua PRIVATE
	Hal
	protocol_static
	$<$<CXX_COMPILER_ID:MSVC>:>$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:pthread>
	common_static
)

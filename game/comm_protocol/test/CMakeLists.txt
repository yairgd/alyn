

add_executable(test1 
	${CMAKE_CURRENT_SOURCE_DIR}/test1.cpp
	${CMAKE_SOURCE_DIR}/game/src/HandleUartMsg.cpp
	${CMAKE_SOURCE_DIR}/game/common/ThreadPool.cpp
	${CMAKE_SOURCE_DIR}/game/common/logger.c

)

target_include_directories(test1 PRIVATE 
	${PROJECT_BINARY_DIR}/	
	${CMAKE_SOURCE_DIR}	
	${CMAKE_SOURCE_DIR}/game/common/
	${CMAKE_CURRENT_SOURCE_DIR}	
)

target_link_libraries(test1 PRIVATE
	Hal
	protocol_static
	#"$<LINK_GROUP:RESCAN, streaming, gui, curl, jsoncpp>"
)



add_executable(load-lua 
	${CMAKE_CURRENT_SOURCE_DIR}/load_lua.cpp
	${CMAKE_SOURCE_DIR}/game/src/HandleUartMsg.cpp
	${CMAKE_SOURCE_DIR}/game/common/ThreadPool.cpp
	${CMAKE_SOURCE_DIR}/game/common/logger.c

)

target_include_directories(load-lua PRIVATE 
	${PROJECT_BINARY_DIR}/	
	${CMAKE_SOURCE_DIR}	
	${CMAKE_SOURCE_DIR}/game/common/
	${CMAKE_CURRENT_SOURCE_DIR}	
)

target_link_libraries(load-lua PRIVATE
	Hal
	protocol_static
	#"$<LINK_GROUP:RESCAN, streaming, gui, curl, jsoncpp>"
)

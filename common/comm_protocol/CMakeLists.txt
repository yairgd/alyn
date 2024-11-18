set ( protocol_SRC 
	${CMAKE_CURRENT_SOURCE_DIR}/protocol-v1/ProtocolStateMachine.cpp
	)


add_library(protocol_static STATIC
	${protocol_SRC}
)




target_include_directories(protocol_static PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/game/


)


target_link_libraries(protocol_static PUBLIC
	Hal
)




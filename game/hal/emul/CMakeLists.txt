
file (GLOB_RECURSE SRC *.cpp
	*.c)



add_library (${PROJECT_NAME}-emul STATIC
	${SRC}

)

target_include_directories(${PROJECT_NAME}-emul PUBLIC . ..)		



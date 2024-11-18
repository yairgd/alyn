project ("Hal")


if (${TARGET} STREQUAL "LINUX_PC") 
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/linux)
elseif(${TARGET} STREQUAL "WINDOWS")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/windows)
elseif(${TARGET} STREQUAL "ZEPHYR")
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/zephyr)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/emul)	


target_include_directories(${PROJECT_NAME}  PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
	..
)

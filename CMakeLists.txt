cmake_minimum_required(VERSION 3.10)


##https://stackoverflow.com/questions/39807664/how-to-best-tell-cmake-where-to-find-dll
##https://discourse.cmake.org/t/copying-config-specific-paths-at-build-time/4776

set(VCPKG_DIR "C:/Users/yairg/vcpkg/installed/x64-windows/")
set (CMAKE_TOOLCHAIN_FILE "C:/Users/yairg/vcpkg/scripts/buildsystems/vcpkg.cmake"  CACHE STRING "")
set (VCPKG_TARGET_TRIPLET  "x64-windows"  CACHE STRING "")

#C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64


set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER_WORKS 1)

project(lvgl)





#set( CMAKE_MESSAGE_LOG_LEVEL "DEBUG" )

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include (${CMAKE_SOURCE_DIR}/cmake/functions.cmake)
set(PRE_CONFIGURE_FILE "${CMAKE_SOURCE_DIR}/cmake/git.h.in")
set(POST_CONFIGURE_FILE "${CMAKE_BINARY_DIR}/git.h")
include (${CMAKE_SOURCE_DIR}/cmake/git_watcher.cmake)
set (CMAKE_CXX_STANDARD 17)


#https://stackoverflow.com/questions/10046114/in-cmake-how-can-i-test-if-the-compiler-is-clang
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set (CMAKE_MODULE_PATH "C:/Users/gadely2/vcpkg/installed/x64-windows/include")
	add_definitions(-DSLEEP=sleep)
	set (CMAKE_C_FLAGS  " -g -O0 ${CMAKE_C_FLAGS_DEBUG_INIT}  ${CMAKE_C_FLAGS_DEBUG}    " )
	set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -std=c++17 -fpermissive")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_definitions(-DSLEEP=Sleep) 
	add_definitions("-D_CRT_SECURE_NO_WARNINGS -DWIN32")
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")  
	set(CMAKE_CXX_FLAGS_DEBUG_INIT " /Zi ")
	set (CMAKE_CXX_FLAGS  " ${CMAKE_CXX_FLAGS_DEBUG_INIT}  ${CMAKE_CXX_FLAGS_DEBUG}  /MDd" )

	set(CMAKE_C_FLAGS_DEBUG_INIT "  /Zi")
	set (CMAKE_C_FLAGS  " ${CMAKE_C_FLAGS_DEBUG_INIT}  ${CMAKE_C_FLAGS_DEBUG}   /MDd " )

	link_directories(
		${VCPKG_DIR}/debug/lib/
	)
	include_directories(
		${VCPKG_DIR}/include
	)
	add_definitions (
		-DNO_SYSLOG
	)
endif()




if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	set (TARGET "LINUX_PC")	
	set (Python3_EXECUTABLE python)	
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set (UART_FILE uart_win.cpp)
	set (TARGET "WINDOWS")	
	set (Python3_EXECUTABLE "C:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Python39_64\\python.exe")
endif()



add_subdirectory(game)





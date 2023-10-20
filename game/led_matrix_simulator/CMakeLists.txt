cmake_minimum_required(VERSION 3.1.0)


project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
set (CMAKE_C_FLAGS_DEBUG "-O0 -g")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -std=c++17 -fpermissive")

add_executable(led-screen-sim
    mainwindow.ui
    mainwindow.cpp
    ledmatrix.cpp
    threadedworker.cpp
    ledcircle.cpp
    main.cpp
   
    #    resources.qrc
)

target_link_libraries(led-screen-sim 
	-Wl,--start-group
	Qt5::Widgets 
	effects_static
	lua
	lua_module
	lua_binaries
	pthread
       -Wl,--end-group
)
target_include_directories(led-screen-sim  PRIVATE
	${CMAKE_SOURCE_DIR}/game/lua-5.4.6/src/
)

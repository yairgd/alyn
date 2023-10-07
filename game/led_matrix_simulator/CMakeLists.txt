cmake_minimum_required(VERSION 3.1.0)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
set (CMAKE_C_FLAGS_DEBUG "-O0 -g")
set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -std=c++17")

add_executable(led-screen-sim
    mainwindow.ui
    mainwindow.cpp
    chessboardwidget.cpp
    main.cpp
    #    resources.qrc
)

target_link_libraries(led-screen-sim Qt5::Widgets effects_static)
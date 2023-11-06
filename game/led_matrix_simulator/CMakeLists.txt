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


# https://stackoverflow.com/questions/61048049/using-qt5-installed-via-vcpkg-in-visual-studio-for-c
# https://github.com/microsoft/vcpkg/discussions/26846

#set (CMAKE_PREFIX_PATH "C:\\Users\\gadely2\\vcpkg\\installed\\x64-windows\\ib")



#find_package(Qt5Core  REQUIRED COMPONENTS system filesystem date_time  PATHS="C:/Users/gadely2/vcpkg/installed/x64-windows/lib" NO_DEFAULT_PATH)
#find_package(Qt5Gui  REQUIRED COMPONENTS system filesystem date_time  PATHS="C:/Users/gadely2/vcpkg/installed/x64-windows/lib")
#find_package(Qt5Widgets  REQUIRED COMPONENTS system filesystem date_time  PATHS="C:/Users/gadely2/vcpkg/installed/x64-windows/lib")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

#find_package(Qt5 COMPONENTS Widgets Concurrent CONFIG REQUIRED)
#find_package(Qt5 COMPONENTS Gui Concurrent CONFIG REQUIRED)
#find_package(Qt5 COMPONENTS Core Concurrent CONFIG REQUIRED)

find_package(Qt5 COMPONENTS Core Widgets CONFIG REQUIRED)
if (${Qt5Core_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()



#find_package(Qt5 COMPONENTS Widgets REQUIRED)



add_executable(led-screen-sim
    mainwindow.ui
    mainwindow.cpp
    ledmatrix.cpp
    threadedworker.cpp
    ledcircle.cpp
    arrawkey.cpp
    keypad.cpp
    main.cpp
    terminaltextedit.cpp
    resources.qrc
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	set(qt_libs 
		Qt5::Widgets
		Qt5::Core
		Qt5::Gui
		pthread
		readline
	)
else()
	set (qt_libs
		Qt5Guid.lib
		Qt5Cored.lib
		Qt5Widgetsd.lib
	)
endif()

target_link_libraries(led-screen-sim 
	 -Wl,--start-group  

	effects_static
	lua
	lua_module
	lua_binaries
	${qt_libs}
         
       -Wl,--end-group
)

target_include_directories(led-screen-sim  PRIVATE
	${CMAKE_SOURCE_DIR}/game/lua-5.4.6/src/
    ${VCPKG_DIR}/include/Qt5/
   	${VCPKG_DIR}/include/Qt5/QtWidgets
	${VCPKG_DIR}/include/Qt5/QtCore
	${VCPKG_DIR}/include/Qt5/QtGui
)



target_compile_definitions(led-screen-sim PRIVATE
	#	-D USE_GUI_CONSOLE
	#	-D LUA_USE_READLINE
)



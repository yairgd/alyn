#cmake_minimum_required(VERSION 3.1.0)



#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#if(CMAKE_VERSION VERSION_LESS "3.7.0")
#	set(CMAKE_INCLUDE_CURRENT_DIR ON)
#endif()


# https://stackoverflow.com/questions/61048049/using-qt5-installed-via-vcpkg-in-visual-studio-for-c
# https://github.com/microsoft/vcpkg/discussions/26846

#set (CMAKE_PREFIX_PATH "C:\\Users\\yairg\\vcpkg\\installed\\x64-windows\\ib")

#	set (Qt5_DIR "${VCPKG_DIR}/share/cmake/Qt5/")

	#message(VCPKG_DIR )
#set (Qt5Widgets_DIR "C:/Users/gadely2/vcpkg/installed/x64-windows//share/cmake/Qt5Widgets/")

#find_package(Qt5Core  REQUIRED COMPONENTS system filesystem date_time  PATHS="C:/Users/gadely2/vcpkg/installed/x64-windows/lib" NO_DEFAULT_PATH)
#find_package(Qt5Gui  REQUIRED COMPONENTS system filesystem date_time  PATHS="C:/Users/gadely2/vcpkg/installed/x64-windows/lib")
#find_package(Qt5Widgets  REQUIRED COMPONENTS system filesystem date_time  PATHS="C:/Users/gadely2/vcpkg/installed/x64-windows/lib")

#find_package(Qt5 COMPONENTS Widgets REQUIRED)

#find_package(Qt5 COMPONENTS Widgets Concurrent CONFIG REQUIRED)
#find_package(Qt5 COMPONENTS Gui Concurrent CONFIG REQUIRED)
#find_package(Qt5 COMPONENTS Core Concurrent CONFIG REQUIRED)

#ind_package(Qt5 COMPONENTS Core Widgets CONFIG REQUIRED)
#if (${Qt5Core_FOUND})
	#message("Found Qt " ${Qt5_VERSION})
#else()
#	message("Couldn't find Qt")
#endif()





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
	find_package(Qt5 COMPONENTS Widgets Concurrent CONFIG REQUIRED)
	find_package(Qt5 COMPONENTS Gui Concurrent CONFIG REQUIRED)
	find_package(Qt5 COMPONENTS Core Concurrent CONFIG REQUIRED)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" )
	set (Qt5_DIR "C:/Users/gadely2/vcpkg/installed/x64-windows//share/cmake/Qt5/")
	find_package(Qt5 COMPONENTS Widgets)
	find_package(Qt5 COMPONENTS Gui)
	find_package(Qt5 COMPONENTS Core)

	if (Qt5Widgets_FOUND)
		message(STATUS "Qt5Widgets found: ${Qt5Widgets_INCLUDE_DIRS}")
	else()
		message(WARNING "Qt5Widgets not found")
	endif()

	message(!!!!!!!!!!!!!!!!!! ${VCPKG_DIR} Info)

	# this file list shoud ebded wih d - like Qt5Guid.lib for debug, the path to debug libraries shoud be added
	set (qt_libs
		Qt5Gui.lib
		Qt5Core.lib
		Qt5Widgets.lib
		readline
	)
	
	target_include_directories(led-screen-sim  PRIVATE
		..
		${Qt5Widgets_INCLUDE_DIRS}

		#C:/Users/gadely2/vcpkg/installed/x64-windows/include/


	#	${VCPKG_DIR}/include/Qt5/
		#${VCPKG_DIR}/include/Qt5/QtWidgets
		#${VCPKG_DIR}/include/Qt5/QtCore
	#	${VCPKG_DIR}/include/Qt5/QtGui
		#C:/Users/gadely2/vcpkg/installed/x64-windows/include/Qt5/QtWidgets
		#C:/Users/gadely2/vcpkg/installed/x64-windows/include/Qt5/QtCore
	#	C:/Users/gadely2/vcpkg/installed/x64-windows/include/Qt5/QtGui
	#	C:/Users/gadely2/vcpkg/installed/x64-windows/include/Qt5/
	#	C:/Users/gadely2/vcpkg/installed/x64-windows/include/
		#${Qt5Core_INCLUDE_DIRS}



	)
endif()

target_link_libraries(led-screen-sim 
	  
		$<$<CXX_COMPILER_ID:MSVC>:>$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--start-group>

	lua
	lua_module
	lua_binaries
	common_static
	luaEffects_static
	effects_static
	Hal
	${qt_libs}
	$<$<CXX_COMPILER_ID:MSVC>:>$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wl,--end-group>

	
)






if (${USE_GUI_CONSOLE})
	target_compile_definitions(led-screen-sim PRIVATE
		-D USE_GUI_CONSOLE
	)
endif()





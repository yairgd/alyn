#cmake_minimum_required(VERSION 3.1.0)



#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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
	find_package(Qt5 COMPONENTS Widgets Concurrent CONFIG REQUIRED)
	find_package(Qt5 COMPONENTS Gui Concurrent CONFIG REQUIRED)
	find_package(Qt5 COMPONENTS Core Concurrent CONFIG REQUIRED)

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


if(TARGET led-screen-sim AND MSVC)
    set(QT_DLL_DIR "${Qt5_DIR}/../../../bin") # Path to the Qt DLLs within the vcpkg installation
	set(DLL_DEST_DIR $<TARGET_FILE_DIR:led-screen-sim>)

    add_custom_command(TARGET led-screen-sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${QT_DLL_DIR}/Qt5Core.dll"
            "${QT_DLL_DIR}/Qt5Gui.dll"
            "${QT_DLL_DIR}/Qt5Widgets.dll"
            ${DLL_DEST_DIR}
    )

	file(GLOB DLL_FILES "C:/Users/gadely2/vcpkg/installed/x64-windows/bin/*.dll")

	# Copy all DLLs to the target directory after build
	add_custom_command(TARGET led-screen-sim POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${DLL_FILES} ${DLL_DEST_DIR}
	)
endif()


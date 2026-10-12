cmake_minimum_required(VERSION 3.13)
project(alyn_sim C CXX)

# libalyn_sim: the MCU C code (alyn common/) as static libraries for the Go TUI (cgo).
# common/ is used in place from the enclosing alyn repo (incl. its lua-5.4.6); nothing is copied or edited.
set(ALYN_ROOT "${CMAKE_SOURCE_DIR}/../.." CACHE PATH "alyn repo root (alyn-go lives in it)")
get_filename_component(ALYN_ROOT "${ALYN_ROOT}" ABSOLUTE)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

# what alyn's top-level CMakeLists.txt provides to common/
set(TARGET LINUX_PC)
set(ZEPHYR FALSE)
# ON: the Lua REPL calls the external readline()/write_line() hooks (console.c), like the
# Qt terminal widget, instead of GNU readline on stdin/stdout.
set(USE_GUI_CONSOLE ON)
set(PROJ_TOP ${ALYN_ROOT})
set(Python3_EXECUTABLE python)
set(msvc_readline "-D LUA_USE_READLINE")
set(LUAC_PATH ${CMAKE_BINARY_DIR}/common/luaEffects/luac/luac)
add_definitions(-DSLEEP=sleep)

add_subdirectory(${ALYN_ROOT}/common ${CMAKE_BINARY_DIR}/common)

add_library(alyn_sim STATIC src/sim.c src/console.c)
target_include_directories(alyn_sim PRIVATE
	${ALYN_ROOT}/common
	${ALYN_ROOT}/common/utils
	${ALYN_ROOT}/common/effects/src
	${ALYN_ROOT}/common/luaEffects/src
	${ALYN_ROOT}/common/luaEffects/lua-5.4.6/src
	${ALYN_ROOT}/common/hal
)

# Go links these (internal/sim, cgo) inside a --start-group.
install(TARGETS alyn_sim lua lua_module lua_binaries common_static luaEffects_static
	effects_static Hal ARCHIVE DESTINATION lib)

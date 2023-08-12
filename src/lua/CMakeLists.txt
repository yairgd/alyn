# https://github.com/zephyrproject-rtos/zephyr/discussions/45485
# Get a list of all source files in the current directory
set(LUA_SOURCES
  lapi.c
  lauxlib.c
  lbaselib.c
  lcode.c
  luac.c
  lgc.c
  lcorolib.c
  lctype.c
  ldebug.c
  ldo.c
  ldump.c
  lfunc.c
  lgc.c
  liolib.c
  llex.c
  lmathlib.c
  lmem.c
  lobject.c
  lopcodes.c
  lparser.c
  lstate.c
  lstring.c
  lstrlib.c
  ltable.c
  ltablib.c
  ltm.c
  lundump.c
  lvm.c
  lzio.c
  linit.c
)

# Add a library for your subdirectory
add_library(lua_module ${LUA_SOURCES})

# Include directories for your subdirectory
target_include_directories(lua_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link any necessary dependencies
target_link_libraries(lua_module zephyr)



/*
** $Id: lbaselib.c $
** Basic library
** See Copyright Notice in lua.h
*/

#define lbaselib_c
#define LUA_LIB

#include "lprefix.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "hwctl.h"
#include "system_model.h"

#include <zephyr/kernel.h>

static int luaB_tostring (lua_State *L) {
  luaL_checkany(L, 1);
  luaL_tolstring(L, 1, NULL);
  return 1;
}

static int luaB_enable (lua_State *L)
  {
  int id = (int) luaL_checknumber (L, -1);
  lua_pop (L, 1);
  hwctl_stop_free_run();	
  if (0 < id && id <= 8)
	hwctl_enable_node(id - 1) ;
  else {
	hwctl_disable_all_nodes();
  }
  return 0;
  }

static int luaB_delay (lua_State *L) {
  int delay_us = (int) luaL_checknumber (L, -1);
  lua_pop (L, 1);

  k_sleep(K_USEC(delay_us));		
  return 0;
}


static int luaB_blink (lua_State *L) {
  int id = (int) luaL_checknumber (L, -1);
  int freq = (int) luaL_checknumber (L, -2);
  int timeout = (int) luaL_checknumber (L, -3);

  lua_pop (L, 3);
  if (0 < id && id <= 8) {
		set_free_run_delay(100);
		start_blink(id - 1,freq,timeout);
		hwctl_start_free_run();
  } else {
		hwctl_disable_all_nodes();		
  }
  return 0;
}



static const luaL_Reg game_funcs[] = {

  {"tostring", luaB_tostring},
  {"enable", luaB_enable},
  {"delay", luaB_delay},
  {"blink", luaB_blink},

  /* placeholders */
  {NULL, NULL}
};



LUAMOD_API int luaopen_game (lua_State *L) {
  luaL_newlib(L, game_funcs);
//	lua_setglobal(L, "game");
#if 0
/* open lib into global table */
  lua_pushglobaltable(L);
  luaL_setfuncs(L, game_funcs, 0);
#endif
  return 1;
}

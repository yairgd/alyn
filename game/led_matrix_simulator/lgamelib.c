/*
** $Id: lbaselib.c $
** Basic library
** See Copyright Notice in lua.h
*/
//
//https://marcbalmer.ch/integrating-lua-with-c-part-2-dc39f1df2ba9
//https://stackoverflow.com/questions/14208951/pass-pure-lua-object-to-c-function-and-get-value
//https://stackoverflow.com/questions/9168058/how-to-dump-a-table-to-console
//https://copyprogramming.com/howto/how-do-i-create-a-class-object-in-lua-c-api-5-2 
//https://stackoverflow.com/questions/55050685/how-to-correctly-redirect-stdout-logging-and-tqdm-into-a-pyqt-widget
#define lbaselib_c
#define LUA_LIB

//#include "lprefix.h"
#include "u8.h"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "led_matrix.h"


static int luaB_tostring (lua_State *L) {
  luaL_checkany(L, 1);
  luaL_tolstring(L, 1, NULL);
  return 1;
}

static int luaB_enable (lua_State *L)
  {
  int id = (int) luaL_checknumber (L, -1);
  lua_pop (L, 1);
 
  return 0;
  }

static int luaB_delay (lua_State *L) {
  int delay_us = (int) luaL_checknumber (L, -1);
  usleep(delay_us);
  lua_pop (L, 1);

  return 0;
}


static int luaB_blink (lua_State *L) {
  int id = (int) luaL_checknumber (L, -1);
  int freq = (int) luaL_checknumber (L, -2);
  int timeout = (int) luaL_checknumber (L, -3);

  lua_pop (L, 3);
}


static int luaB_banner_print(lua_State *L) {
  char * str = (char*) luaL_checkstring (L, -1);
  int len, unicode;

  lua_pop (L, 1);

  while (*str) {
  	len = u8_to_unicode(str, &unicode );
	printf("%x\n", unicode);
	str += len;
		
  }
  return 0;
}


static int luaB_plot(lua_State *L) {
  int x = (int) luaL_checknumber (L, -3);
  int y  = (int) luaL_checknumber (L, -2);
  int c  = (int) luaL_checknumber (L, -1);

  lua_pop (L, 3);
	struct led_matrix * led_matrix = get_led_matrix();
	canvas_plot(&led_matrix->canvas,x,y,c);
  return 0;
}

static int luaB_line(lua_State *L) {
  int x1 = (int) luaL_checknumber (L, -5);
  int y1  = (int) luaL_checknumber (L, -4);
  int x2 = (int) luaL_checknumber (L, -3);
  int y2  = (int) luaL_checknumber (L, -2);
  int c  = (int) luaL_checknumber (L, -1);

  lua_pop (L, 5);
  struct led_matrix * led_matrix = get_led_matrix();
  canvas_line(&led_matrix->canvas,x1,y1,x2,y2,c);
  return 0;
}

static int luaB_circle(lua_State *L) {
  int x  = (int) luaL_checknumber (L, -4);
  int y  = (int) luaL_checknumber (L, -3);
  int r  = (int) luaL_checknumber (L, -2);
  int c  = (int) luaL_checknumber (L, -1);

  lua_pop (L, 4);
  struct led_matrix * led_matrix = get_led_matrix();
  canvas_circle(&led_matrix->canvas,x,y,r,c);
  return 0;
}


static int luaB_fill_circle(lua_State *L) {
  int x  = (int) luaL_checknumber (L, -4);
  int y  = (int) luaL_checknumber (L, -3);
  int r  = (int) luaL_checknumber (L, -2);
  int c  = (int) luaL_checknumber (L, -1);

  lua_pop (L, 4);
  struct led_matrix * led_matrix = get_led_matrix();
  canvas_fill_circle(&led_matrix->canvas,x,y,r,c);
  return 0;
}

static int luaB_clean(lua_State *L) {
  int x  = (int) luaL_checknumber (L, -4);
  int y  = (int) luaL_checknumber (L, -3);
  int w  = (int) luaL_checknumber (L, -2);
  int h  = (int) luaL_checknumber (L, -1);

  lua_pop (L, 4);
  struct led_matrix * led_matrix = get_led_matrix();
  canvas_clean(&led_matrix->canvas, &RECT(x,y,w,h)) ;

  return 0;
}







static const luaL_Reg game_funcs[] = {

  {"tostring", luaB_tostring},
  {"enable", luaB_enable},
  {"delay", luaB_delay},
  {"blink", luaB_blink},
  {"banner_print", luaB_banner_print},
  {"plot", luaB_plot},
  {"line", luaB_line},
  {"circle", luaB_circle},
  {"fill_circle", luaB_fill_circle},
  {"clean", luaB_clean},

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

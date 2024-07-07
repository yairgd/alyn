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
//https://stackoverflow.com/questions/6137684/iterate-through-lua-table
//http://lua-users.org/lists/lua-l/2007-01/msg00458.html call c function pointed by table
//https://stackoverflow.com/questions/40551641/call-lua-table-function-from-c-w-self-in-function
//
#define lbaselib_c
#define LUA_LIB

#include "u8.h"
#include "obj.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"
#include "led_matrix.h"
#include "timing.h"
#include "system_model.h"

static int led_matrix_channel_id = 0;

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
//	led_matrix_merge(led_matrix_get());			
	timing_sleep(delay_us);
	lua_pop (L, 1);

	return 0;
}


static int luaB_blink (lua_State *L) {
	int id = (int) luaL_checknumber (L, -3);
	int freq = (int) luaL_checknumber (L, -2);
	int blink_time = (int) luaL_checknumber (L, -1);
	lua_pop (L, 3);

	start_blink( id,  freq,  blink_time);
	return 0;
}

static int luaB_led_rgb (lua_State *L) {
	int id = (int) luaL_checknumber (L, -4);
	int r = (int) luaL_checknumber (L, -3);
	int g = (int) luaL_checknumber (L, -2);
	int b = (int) luaL_checknumber (L, -1);

	lua_pop (L, 4);

	set_rgb(id,r,g,b);
	return 0;
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
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  led_matrix_channel_id);    	
	canvas_plot(&channel->canvas,x,y,c);
	return 0;
}

static int luaB_line(lua_State *L) {
	int x1 = (int) luaL_checknumber (L, -5);
	int y1  = (int) luaL_checknumber (L, -4);
	int x2 = (int) luaL_checknumber (L, -3);
	int y2  = (int) luaL_checknumber (L, -2);
	int c  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 5);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  led_matrix_channel_id);    	
	canvas_line(&channel->canvas,x1,y1,x2,y2,c);
	return 0;
}

static int luaB_circle(lua_State *L) {
	int x  = (int) luaL_checknumber (L, -4);
	int y  = (int) luaL_checknumber (L, -3);
	int r  = (int) luaL_checknumber (L, -2);
	int c  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 4);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  led_matrix_channel_id);    	
	canvas_circle(&channel->canvas,x,y,r,c);
	return 0;
}


static int luaB_fill_circle(lua_State *L) {
	int x  = (int) luaL_checknumber (L, -4);
	int y  = (int) luaL_checknumber (L, -3);
	int r  = (int) luaL_checknumber (L, -2);
	int c  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 4);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  led_matrix_channel_id);    	
	canvas_fill_circle(&channel->canvas,x,y,r,c);
	return 0;
}

static int luaB_clean_rect(lua_State *L) {
	int x  = (int) luaL_checknumber (L, -4);
	int y  = (int) luaL_checknumber (L, -3);
	int w  = (int) luaL_checknumber (L, -2);
	int h  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 4);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  led_matrix_channel_id);    	
	canvas_clean_rect(&channel->canvas, &RECT(x,y,w,h)) ;

	return 0;
}
static int luaB_clean(lua_State *L) {
	for (int  i = 0;i < 3;i++) {
		struct channel * channel = led_matrix_get_channel(led_matrix_get(),  i);  
		canvas_clean(&channel->canvas) ;		  
	}
	return 0;
}

static int luaB_opacity(lua_State *L) {
	double o1 = (double) luaL_checknumber (L, -3);
	double o2 = (double) luaL_checknumber (L, -2);
	double o3 = (double) luaL_checknumber (L, -1);
	led_matrix_set_opcaity(led_matrix_get(), o1,o2,o3);
	return 0;
}





static int luaB_select_channel(lua_State *L) {
	int channel  = (int) luaL_checknumber (L, -1);
	lua_pop (L, 1);
	led_matrix_channel_id = channel;
	return 0;
}

static int luaB_setkey(lua_State *L) {
	int id  = (int) luaL_checknumber (L, -2);
	int v  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 2);
	set_key(id,v);
	return 0;
}

static int luaB_setlongkey(lua_State *L) {
	int id  = (int) luaL_checknumber (L, -2);
	int v  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 2);
	set_long_key(id,v);
	return 0;
}


static int luaB_rotate(lua_State *L) {
	int n  = (int) luaL_checknumber (L, -1);

	lua_pop (L, 1);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  led_matrix_channel_id);    	
	if (n > 0) 
		canvas_rotate_left(&channel->canvas, n) ;
	else if (n<0)
		canvas_rotate_right(&channel->canvas, -n) ;



	return 0;
}





static int luaB_stop_reason(lua_State *L) {
	int id  = (int) luaL_checknumber (L, -1);
	lua_pop (L, 1);

	lua_pushinteger(L, get_stop_reason(id));
	return 1;
}

static int luaB_keystatus(lua_State *L) {
	lua_pushinteger(L, get_keys());
	return 1;

}


static int luaB_longkeystatus(lua_State *L) {
	lua_pushinteger(L, get_long_keys());
	return 1;

}





static const luaL_Reg game_funcs[] = {

	{"tostring", luaB_tostring},
	{"enable", luaB_enable},
	{"delay", luaB_delay},
	{"blink", luaB_blink},
	{"led_rgb", luaB_led_rgb},
	{"banner_print", luaB_banner_print},
	{"plot", luaB_plot},
	{"line", luaB_line},
	{"circle", luaB_circle},
	{"fill_circle", luaB_fill_circle},
	{"clean_rect", luaB_clean_rect},
	{"clean", luaB_clean},
	{"select_channel", luaB_select_channel},
	{"opacity", luaB_opacity},
	{"stop_reason", luaB_stop_reason},
	{"keys", luaB_keystatus},
	{"set_key", luaB_setkey},
	{"long_keys", luaB_longkeystatus},
	{"set_long_key", luaB_setlongkey},
	{"rotate", luaB_rotate},





	/* placeholders */
	{NULL, NULL}
};



LUAMOD_API int luaopen_game (lua_State *L) {
	luaL_newlib(L, game_funcs);
	register_frame_effect_class(L);
	register_banner1_effect_class(L);
	register_banner2_effect_class(L);
	register_rect_effect_class(L);

	//	lua_setglobal(L, "game");
#if 0
	/* open lib into global table */
	lua_pushglobaltable(L);
	luaL_setfuncs(L, game_funcs, 0);
#endif
	return 1;
}

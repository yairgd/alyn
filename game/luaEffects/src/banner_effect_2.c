/*
 * =====================================================================================
 *
 *       Filename:  lua_banner_effect.c
 *
 *    Description:  lua wrpper to banner effect - >reffer here: https://copyprogramming.com/howto/how-do-i-create-a-class-object-in-lua-c-api-5-2 
 *
 *        Version:  1.0
 *        Created:  10/11/2023 05:22:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "banner.h"
#include "led_matrix.h"
#include "obj.h"
#include "font.h"
#include "lua_memory.h"


#define LUA_BANNER "banner2"

static void config(lua_State *L, struct banner * banner,struct rect * rect  ) {
	const struct font *  fnt = 0 ;
	char * txt  = (char *) luaL_checkstring (L, -6);
 	int start_idx  = (int) luaL_checknumber (L, -5);
 	int end_idx  = (int) luaL_checknumber (L, -4);
 	double on_time  = (double) luaL_checknumber (L, -3);
 	double off_time  = (double) luaL_checknumber (L, -2);
 	int font  = (int) luaL_checknumber (L, -1);


	switch(font) {
		case 0: fnt = font_6x13();break;
		case 1: fnt = font_9x18();break;
		default: fnt =0;break;
	};
	if (fnt == 0)
		 luaL_error(L, "no valid font type");    		
	banner_init_with_text(banner, *rect, fnt,txt);
	effect_set_config (&banner->effect, BANNER_CONFIG_BLINK(start_idx,end_idx,on_time,off_time) );
	

}
static int lua_banner_gc(lua_State *L) {
	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_BANNER);

	lua_free(user_data->data);
	lua_free(user_data);
	return 0;
}
static int lua_banner_render(lua_State *L) {
	int channel_id  = (int) luaL_checknumber (L, -1);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  channel_id);    	


	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_BANNER);
	struct banner * banner = (struct banner * )user_data->data;

	effect_render(&banner->effect, &channel->canvas, 0); 
	return 0;
}
static int lua_banner_config(lua_State *L) {
	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_BANNER);
	struct banner * banner = (struct banner * )user_data->data;

	struct lua_user_data * rect = *(struct lua_user_data**)luaL_checkudata(L, 2, "rect");

	//char  * txt = *(char**)luaL_checkudata(L, 3, "string");


	if (rect->type != obj_type_rect)
		 luaL_error(L, "This is not a valid rectange");   

	config(L, banner,rect->data);

	return 0;
}



static int lua_banner_new(lua_State* L) {
	struct lua_user_data  * user_data = object_new(obj_type_banner);	
	struct banner * banner = (struct banner * )user_data->data;	
	struct lua_user_data * rect = *(struct lua_user_data**)luaL_checkudata(L, 1, "rect");
	
	if (rect->type != obj_type_rect)
		 luaL_error(L, "This is not a valid rectange");    

	config(L, banner,rect->data);
	

	*(struct lua_user_data**)lua_newuserdata(L, sizeof(struct lua_user_data*)) = user_data;
	luaL_setmetatable(L, LUA_BANNER);
	return 1;
}

static int lua_banner_index(lua_State *L) {
	int i = luaL_checkinteger(L, 2);
	lua_pushinteger(L, i);
	return 1;
}

static int lua_banner_newindex(lua_State* L) {
	return 0;
}

int register_banner2_effect_class(lua_State *L) {
	// instance functions
	static const luaL_Reg meta[] =
	{   { "__gc"        ,lua_banner_gc          },
		{ NULL          ,NULL            }  };
	static const luaL_Reg meth[] =
	{   { "render" ,lua_banner_render },
	    { "config" ,lua_banner_config },
	    { NULL          ,NULL         }  };
	luaL_newmetatable(L, LUA_BANNER);
	luaL_setfuncs    (L, meta, 0);
	luaL_newlib      (L, meth);
	lua_setfield     (L, -2, "__index");
	lua_pop          (L, 1);
	// static functions
	static const luaL_Reg static_meta[] =
	{   { "__index" ,lua_banner_index },
		{ "__call"  ,lua_banner_new   },
		{ NULL      ,NULL      }  };
	static const luaL_Reg static_meth[] =
	{   { "new"     ,lua_banner_new   },
		{ NULL      ,NULL      }  };
	luaL_newlib      (L, static_meth);
	luaL_newlib      (L, static_meta);
	lua_setmetatable (L, -2);
	lua_setglobal(L, LUA_BANNER);

	return 1;
}

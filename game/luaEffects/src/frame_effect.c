/*
 * =====================================================================================
 *
 *       Filename:  lua_frame_effect.c
 *
 *    Description:  lua wrpper to frame effect - >reffer here: https://copyprogramming.com/howto/how-do-i-create-a-class-object-in-lua-c-api-5-2 
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

#include "frame.h"
#include "led_matrix.h"
#include "obj.h"
#include "lua_memory.h"



#define LUA_FRAME "frame"

static int lua_frame_gc(lua_State *L) {
	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_FRAME);

	lua_free(user_data->data);
	lua_free(user_data);
	return 0;
}

static int lua_frame_render(lua_State *L) {
	int channel_id  = (int) luaL_checknumber (L, -1);
	struct channel * channel = led_matrix_get_channel(led_matrix_get(),  channel_id);    	
	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_FRAME);
	struct frame * frame = (struct frame * )user_data->data;

	effect_render(&frame->effect, &channel->canvas, 0); 
	return 0;

}

static int lua_frame_new(lua_State* L) {
	struct lua_user_data  * user_data = object_new(obj_type_frame);	
	struct frame * frame = (struct frame * )user_data->data;	
	struct lua_user_data * rect = *(struct lua_user_data**)luaL_checkudata(L, 1, "rect");

	if (rect->type != obj_type_rect)
		 luaL_error(L, "This is not a valid rectange");    

	int c1  = (int) luaL_checknumber (L, -5);
	int c2  = (int) luaL_checknumber (L, -4);
	int color_rate_change  = (int) luaL_checknumber (L, -3);
	int pixel_change  = (int) luaL_checknumber (L, -2);
	int dir  = (int) luaL_checknumber (L, -1);


	effect_set_config (&frame->effect,  FRAME_CONFIG_TYPE_1(c1,c2,color_rate_change,pixel_change, dir,*(struct rect *)rect->data ));
	//int i = 1 + lua_istable(L, 1);
	//foo->x = !lua_isnoneornil(L, i) ? luaL_checkinteger(L, i) : 0;
	*(struct lua_user_data**)lua_newuserdata(L, sizeof(struct lua_user_data*)) = user_data;
	luaL_setmetatable(L, LUA_FRAME);
	return 1;
}

static int lua_frame_config(lua_State* L) {
	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_FRAME);
	struct frame * frame = (struct frame * )user_data->data;

	struct lua_user_data * rect = *(struct lua_user_data**)luaL_checkudata(L, 2, "rect");
	if (rect->type != obj_type_rect)
		 luaL_error(L, "This is not a valid rectange");    

	int c1  = (int) luaL_checknumber (L, -5);
	int c2  = (int) luaL_checknumber (L, -4);
	int color_rate_change  = (int) luaL_checknumber (L, -3);
	int pixel_change  = (int) luaL_checknumber (L, -2);
	int dir  = (int) luaL_checknumber (L, -1);


	effect_set_config (&frame->effect,  FRAME_CONFIG_TYPE_1(c1,c2,color_rate_change,pixel_change, dir,*(struct rect *)rect->data ));

}
static int lua_frame_index(lua_State *L) {
	int i = luaL_checkinteger(L, 2);
	lua_pushinteger(L, i);
	return 1;
}

int lua_frame_newindex(lua_State* L) {
	return 0;
}

int register_frame_effect_class(lua_State *L) {
	// instance functions
	static const luaL_Reg meta[] =
	{   { "__gc"        ,lua_frame_gc          },
		{ NULL          ,NULL            }  };
	static const luaL_Reg meth[] =
	{   { "render" ,lua_frame_render },
		 { "config" ,lua_frame_config },
		{ NULL          ,NULL            }  };
	luaL_newmetatable(L, LUA_FRAME);
	luaL_setfuncs    (L, meta, 0);
	luaL_newlib      (L, meth);
	lua_setfield     (L, -2, "__index");
	lua_pop          (L, 1);
	// static functions
	static const luaL_Reg static_meta[] =
	{   { "__index" ,lua_frame_index },
		{ "__call"  ,lua_frame_new   },
		{ NULL      ,NULL      }  };
	static const luaL_Reg static_meth[] =
	{   { "new"     ,lua_frame_new   },
		{ NULL      ,NULL      }  };
	luaL_newlib      (L, static_meth);
	luaL_newlib      (L, static_meta);
	lua_setmetatable (L, -2);
	lua_setglobal(L, LUA_FRAME);

	return 1;
}

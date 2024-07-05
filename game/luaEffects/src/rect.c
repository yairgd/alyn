/*
 * =====================================================================================
 *
 *       Filename:  lua_rect_effect.c
 *
 *    Description:  lua wrpper to rect effect - >reffer here: https://copyprogramming.com/howto/how-do-i-create-a-class-object-in-lua-c-api-5-2 
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

#include "obj.h"
#include "canvas.h"
#include "lua_memory.h"



#define LUA_FRAME "rect"

static int lua_rect_gc(lua_State *L) {
	struct lua_user_data * user_data = *(struct lua_user_data**)luaL_checkudata(L, 1, LUA_FRAME);

	// use this code , if there is nome thing to free in rectnagle (it is an example, there is no what to free in rect)
	//	struct rect * rect = (struct rect * )user_data->data;
	//	rect_free(rect);

	lua_free(user_data->data);
	lua_free(user_data);
	return 0;
}

static int lua_rect_new(lua_State* L) {

	struct lua_user_data  * user_data = object_new(obj_type_rect);	
	int x  = (int) luaL_checknumber (L, -4);
	int y = (int) luaL_checknumber (L, -3);
  	int w  = (int) luaL_checknumber (L, -2);
  	int h  = (int) luaL_checknumber (L, -1);
	*((struct rect *)user_data->data) = (struct rect) {x,y,w,h};

	//int i = 1 + lua_istable(L, 1);
	//foo->x = !lua_isnoneornil(L, i) ? luaL_checkinteger(L, i) : 0;
	*(struct lua_user_data**)lua_newuserdata(L, sizeof(struct lua_user_data*)) = user_data;
	luaL_setmetatable(L, LUA_FRAME);
	return 1;
}

static int lua_rect_index(lua_State *L) {
	int i = luaL_checkinteger(L, 2);
	lua_pushinteger(L, i);
	return 1;
}

int lua_rect_newindex(lua_State* L) {
	return 0;
}

int register_rect_effect_class(lua_State *L) {
	// instance functions
	static const luaL_Reg meta[] =
	{   { "__gc"        ,lua_rect_gc          },
		{ NULL          ,NULL            }  };
	static const luaL_Reg meth[] =
	{ /*  { "render" ,lua_rect_render },*/
		{ NULL          ,NULL            }  };
	luaL_newmetatable(L, LUA_FRAME);
	luaL_setfuncs    (L, meta, 0);
	luaL_newlib      (L, meth);
	lua_setfield     (L, -2, "__index");
	lua_pop          (L, 1);
	// static functions
	static const luaL_Reg static_meta[] =
	{   { "__index" ,lua_rect_index },
		{ "__call"  ,lua_rect_new   },
		{ NULL      ,NULL      }  };
	static const luaL_Reg static_meth[] =
	{   { "new"     ,lua_rect_new   },
		{ NULL      ,NULL      }  };
	luaL_newlib      (L, static_meth);
	luaL_newlib      (L, static_meta);
	lua_setmetatable (L, -2);
	lua_setglobal(L, LUA_FRAME);

	return 1;
}

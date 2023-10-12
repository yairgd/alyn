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

#include "frame_lua_effect.h"
#include "frame.h"
#include "led_matrix.h"
#include "effect.h"



#define LUA_FRAME "frame"

static int lua_frame_gc(lua_State *L) {
    //printf("## __gc\n");
    struct lua_frame_effect *foo = *(struct lua_frame_effect**)luaL_checkudata(L, 1, LUA_FRAME);
    free(foo->effect);
    free(foo);
    return 0;
}
static int lua_frame_render(lua_State *L) {
//    printf("## doSomething\n");
    struct lua_frame_effect *lua_effect = *(struct lua_frame_effect**)luaL_checkudata(L, 1, LUA_FRAME);
    struct led_matrix * matrix = get_led_matrix();    
//    led_matrix_manage(led_matrix);
   effect_render(lua_effect->effect, &matrix->canvas, 0); 
  //  foo->x=123;
//    lua_pushinteger(L, foo->x);
  //  return 1;
  return 0;
}
static int lua_frame_new(lua_State* L) {
//    printf("## new\n");
    struct lua_frame_effect  *foo = malloc(sizeof(struct lua_frame_effect));
    struct led_matrix * led_matrix = get_led_matrix();
    foo->effect = led_matrix_get_frame(led_matrix);
    effect_set_config (foo->effect,  FRAME_CONFIG_TYPE_1(RGB(255,0,255), RGB(255,255,0), 5,20, RECT(0,0,64,32) ));
    

    //int i = 1 + lua_istable(L, 1);
    //foo->x = !lua_isnoneornil(L, i) ? luaL_checkinteger(L, i) : 0;
    *(struct lua_frame_effect**)lua_newuserdata(L, sizeof(struct lua_frame_effect*)) = foo;
    luaL_setmetatable(L, LUA_FRAME);
    return 1;
}
static int lua_frame_index(lua_State *L) {
 //   printf("## index\n");
    int i = luaL_checkinteger(L, 2);
    lua_pushinteger(L, i);
    return 1;
}

int lua_frame_newindex(lua_State* L) {
//  printf("## __newindex\n");
  return 0;
}

#if 0
static const luaL_Reg _meta[] = {
    {"__gc", lua_frame_gc},
    {"__index", lua_frame_index},
    {"__newindex", lua_frame_newindex},
    { NULL, NULL }
};
static const luaL_Reg _methods[] = {
    {"new", lua_frame_new},
    {"dosomething", lua_frame_doSomething},
    { NULL, NULL }
};



int register_frame_effect_class1(lua_State *L) {
	int lib_id, meta_id;
  /* newclass = {} */
  lua_createtable(L, 0, 0);
  lib_id = lua_gettop(L);
  /* metatable = {} */
  luaL_newmetatable(L, LUA_FRAME);
  meta_id = lua_gettop(L);
  luaL_setfuncs(L, _meta, 0);
  /* metatable.__index = _methods */
  luaL_newlib(L, _methods);
  lua_setfield(L, meta_id, "__index");  
  /* metatable.__metatable = _meta */
  luaL_newlib(L, _meta);
  lua_setfield(L, meta_id, "__metatable");
  /* class.__metatable = metatable */
  lua_setmetatable(L, lib_id);
  /* _G["Foo"] = newclass */
  lua_setglobal(L, LUA_FRAME);
}
#endif

int register_frame_effect_class(lua_State *L) {
    // instance functions
    static const luaL_Reg meta[] =
    {   { "__gc"        ,lua_frame_gc          },
        { NULL          ,NULL            }  };
    static const luaL_Reg meth[] =
    {   { "render" ,lua_frame_render },
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

/*
 * =====================================================================================
 *
 *       Filename:  lua_frame_effect.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2023 05:22:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef LUA_EFFECT_H
#define LUA_EFFECT_H 
#include "effect.h"
#include "lauxlib.h"
#include "lualib.h"


int frame_effect_gc();
int frame_effect_index();
int frame_effect_newindex();
int frame_effect_dosomething();
int frame_effect_new();
struct lua_frame_effect {
  struct effect_base * effect;
};

int register_frame_effect_class(lua_State* L);
#endif

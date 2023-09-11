/*
 * =====================================================================================
 *
 *       Filename:  luasrc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/04/2023 07:47:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef LUA_SOURCE_H
#define LUA_SOURCE_H
struct lua_game_data {
	int size;
	const unsigned char * code;
	char name[32];
};


struct lua_game_data  lua_game1 = {
	.name = "first game",	
	.code = ( const unsigned char []) {
#include "game1.lua.h"
	},
	.size = sizeof(( const unsigned char []) {
#include "game1.lua.h"
	})	
};




#endif

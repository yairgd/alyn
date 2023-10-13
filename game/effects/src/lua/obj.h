/*
 * =====================================================================================
 *
 *       Filename:  obj.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/13/2023 03:41:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef OBJ_H
#define OBJ_H 
#include "lauxlib.h"
#include "lualib.h"

#include "banner.h"
#include "frame.h"
#include "canvas.h"

struct lua_user_data {
		enum obj_type {
			obj_type_frame,
			obj_type_banner,
			obj_type_rect,
		} type;
		void * data;
	};




struct lua_user_data * led_matrix_new_object(enum obj_type type);

int register_frame_effect_class(lua_State* L);
int register_banner1_effect_class(lua_State* L);
int register_banner2_effect_class(lua_State* L);
int register_rect_effect_class(lua_State* L);




#endif

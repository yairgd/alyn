/*
 * =====================================================================================
 *
 *       Filename:  obj.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/13/2023 03:40:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "obj.h"
#include <stdlib.h>

struct lua_user_data * object_new(enum obj_type type) {
	struct lua_user_data * obj = malloc(sizeof(struct lua_user_data));

	switch (type) {
		case obj_type_frame:
			obj->data = (void *)frame_new();
			obj->type = obj_type_frame;
			break;

		case obj_type_banner:
			obj->data = (void*)banner_new();
			obj->type = obj_type_banner;
			break;
		
		case obj_type_rect:
			obj->data = (void*)malloc(sizeof (struct rect));
			obj->type = obj_type_rect;
			break;
		
		default:
			obj = 0;
			break;
	};
	return obj;
}


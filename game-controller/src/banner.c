/*
 * =====================================================================================
 *
 *       Filename:  banner.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2023 08:01:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "banner.h"
#include "canvas.h"
#include "u8.h"
#include <assert.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


int rect_width(struct rect * r) {
	return r->buttom_right_x - r->top_left_x;

}
int rect_height(struct rect * r){
	return r->buttom_right_y - r->top_left_y;	
}


struct  canvas *  banner_get_canvas(struct banner * banner)
{
	return &banner->canvas;
}
/**
 * Created  09/25/2023
 * @brief   init banner with external canvsa (pre defined canvas)
 * @note  
 * @param   banner ponter
 * @param   canvs pointer to canvs
 * @param   canvas_effect pointer to effect function on the givven canvs
 * @return  
 */
void banner_init_by_canvas(struct banner * banner, void (*do_effect)(struct canvas *)) {
	banner->effect_id = 3;
	banner->effet.canvas.do_effect	= do_effect;
	do_effect (&banner->canvas);
}


/**
 * Created  09/25/2023
 * @brief  initilize banner  
 * @note  
 * @param    
 * @return  
 */
void banner_init_with_text(struct banner * banner,const struct font * font, int x, int y,const char* fmt, ...){ 
	va_list args;
	va_start(args, fmt);

	vsnprintf(banner->text,sizeof(banner->text), fmt, args);
	int len = u8_strlen(banner->text);
	banner->x = x;
	banner->y = y;
	canvas_init(&banner->canvas,font, len * font_width(font,0) , font_height(font) );
	canvas_print(&banner->canvas, x,y, banner->text);
}

void banner_set_text(struct banner * banner, int x, int y,const char* fmt, ...){ 
	va_list args;
	va_start(args, fmt);
	vsnprintf(banner->text,sizeof(banner->text), fmt, args);
}



/**
 * Created  09/26/2023
 * @brief   place data in the rectnage buffer and rerturn pointer of it.
 * The recangle buffer should be displyed on screen
 * @note  
 * @param   
 * @return  
 */
void banner_get_buffer(struct banner * banner, struct rect * r, char *rect_buffer) {
	int w = r->buttom_right_x - r->top_left_x ;



	for (int y = r->top_left_y; y < r->buttom_right_y && y < banner->canvas.height; y ++) {
		for (int x = r->top_left_x; x < r->buttom_right_x && x < banner->canvas.width; x++) {
			int pixel = *(int*)&banner->canvas.buffer[4 * (x) + 4 * (y) * banner->canvas.width];
			*(int*)&rect_buffer[4 * (x - r->top_left_x) + 4 * (y - r->top_left_y) * w ] = pixel;
		}
	}
}

void banner_blink_effect(struct banner * banner, int rate, int start_idx, int end_idx, int tick_time) {
	banner->effet.blink.rate = rate;
	banner->effet.blink.start_idx = start_idx;
	banner->effet.blink.end_idx = end_idx;
	banner->effet.blink.tick_time = tick_time;
	banner->effet.blink.cnt	= 0;
	banner->effect_id = 1;
	banner->effet.blink.on = 1;

}

void banner_rotate_effect(struct banner * banner, int direction, int step, int tick_time) {
	banner->effect_id = 2;
	banner->effet.rotate.direction = direction;
	banner->effet.rotate.tick_time = tick_time;
	banner->effet.rotate.step = step;
	banner->effet.rotate.cnt = 0;

}

static void bannner_manage_blink(struct banner * banner) 
{
	banner->effet.blink.cnt++;

	if (banner->effet.blink.cnt >= banner->effet.blink.rate) {
		banner->effet.blink.cnt = 0;
		if (banner->effet.blink.on == 1) {
			banner->effet.blink.on = 0;
			if (banner->effet.blink.end_idx !=0 && banner->effet.blink.start_idx <= banner->effet.blink.end_idx)
				// blink range of letters
				canvas_fill_rect(&banner->canvas, &(struct rect  ){banner->x + banner->effet.blink.start_idx * font_width(banner->canvas.font,0) ,banner->y,banner->effet.blink.end_idx * font_width(banner->canvas.font,0) , font_height(banner->canvas.font)},0);

			else   { // blink all text
				int len = u8_strlen(banner->text);				
				canvas_fill_rect(&banner->canvas, &(struct rect  ){banner->x,banner->y,len * font_width(banner->canvas.font,0) , font_height(banner->canvas.font)},0);
			}
		}
		else {
			banner->effet.blink.on = 1;
			canvas_print(&banner->canvas, banner->x,banner->y, banner->text);
		}
	}
}

static void bannner_manage_rotate(struct banner * banner) {
	banner->effet.rotate.cnt++;

	if (banner->effet.rotate.cnt >= banner->effet.rotate.tick_time) {
		banner->effet.rotate.cnt = 0;
		if (banner->effet.rotate.direction == 1) {
			canvas_rotate_left(&banner->canvas,banner->effet.rotate.step);	
		} else if (banner->effet.rotate.direction == 2) {
			canvas_rotate_right(&banner->canvas,banner->effet.rotate.step);				
		}
	}

}


/**
 * Created  09/26/2023
 * @brief   This routine is invoked upon each timeout expiration to update the banner state in accordance with its configuration
 * @note  
 * @param   
 * @return  
 */
void banner_manage(struct banner * banner) {
	switch (banner->effect_id) {
		case 1: // blink
			bannner_manage_blink(banner);
			break;
		case 2: // rotate
			bannner_manage_rotate(banner);
			break;
		case 3:
			banner->effet.canvas.do_effect(&banner->canvas);
			break;
		default:
			// do nothing
			break;
	};
}


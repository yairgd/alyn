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
#include "effect.h"

#include <assert.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/timing.h"
#include "utils/lua_memory.h"

void banner_set_text(struct banner * banner,const char* fmt, ...){ 
	va_list args;
	va_start(args, fmt);
	vsnprintf(banner->text[0],sizeof(banner->text[0]), fmt, args);
}


static void banner_blink_effect(struct banner * banner, int start_idx, int end_idx, double time_on, double time_off) {
	int  len = u8_strlen(banner->text[0]) * font_width(banner->font,0);

	banner->effect.config_id = 1;
	banner->config.blink.start_idx = start_idx;
	banner->config.blink.end_idx = end_idx;
	banner->config.blink.time_on = time_on;
	banner->config.blink.time_off = time_off;



}

static void banner_rotate_effect(struct banner * banner, int direction, int step) {
	banner->effect.config_id = 2;	
	banner->config.rotate.direction = direction;
	banner->config.rotate.step = step;

}


static void bannner_manage_blink(struct effect_base * e, struct canvas * canvas) 
{
	struct banner * banner = (struct banner *)e  ;
	int  len = u8_strlen(banner->text[0]) * font_width(banner->font,0);
	struct rect out_rect = {0,0,len,font_height(banner->font)};
	canvas_set_font(canvas, banner->font);
	canvas_set_rect(canvas, &e->r, &out_rect);
	char text[256];

	if ( banner->config.blink.time_off ==0) {
		// always on
		canvas_print(canvas, 0,0, banner->text[0]);		
	} else {
		if (banner->on == 1 && timing_elapse(banner->start_time, banner->config.blink.time_on) ) {
			banner->on = 0;
			banner->start_time = timing_begin_to_measure_time();
			if (banner->config.blink.end_idx !=0 && banner->config.blink.start_idx <= banner->config.blink.end_idx) {
				// blink range of letters
				int w = banner->config.blink.end_idx  - banner->config.blink.start_idx  + 1;				
				canvas_fill_rect(canvas, &(struct rect  ){banner->config.blink.start_idx * font_width(banner->font,0) ,0,w * font_width(banner->font,0) , font_height(banner->font)},0);
			}
			else   { // blink all text
				int len = u8_strlen(banner->text[0]);				
				canvas_fill_rect(canvas, &(struct rect  ){0,0,len * font_width(banner->font,0) , font_height(banner->font)},0);
			}
		}
		else if (banner->on == 0 && timing_elapse(banner->start_time, banner->config.blink.time_off) ) {
			banner->on = 1;
			banner->start_time = timing_begin_to_measure_time();		
			canvas_print(canvas, 0,0, banner->text[0]);
		} 
	}

	canvas_set_global_rect(canvas);	

}

static void bannner_manage_rotate(struct effect_base * e, struct canvas * canvas) {
	struct banner * banner = (struct banner *)e  ;
	int  len = u8_strlen(banner->text[0]) * font_width(banner->font,0);
	struct rect out_rect = {0,0,len,font_height(banner->font)};
	canvas_set_font(canvas, banner->font);
	canvas_clean(canvas);
	canvas_set_rect(canvas, &e->r, &out_rect);
	
	if (banner->config.rotate.direction == 1) {	
		banner->x--;	
	} else if (banner->config.rotate.direction == 2) {
		banner->x++;
	}

	canvas_print(canvas, banner->x,banner->y, banner->text[0]);
	canvas_set_global_rect(canvas);

}



static void banner_render(struct effect_base * e,  struct canvas * canvas, struct  rect * r) {
	struct banner * banner = (struct banner *)e  ;

	switch (banner->effect.config_id) {
		case 1: // blink
			bannner_manage_blink(e, canvas);
			break;
		case 2: // rotate
			bannner_manage_rotate(e, canvas);
			break;
		default:
			// do nothing
			break;
	};
}



static void banner_config(struct effect_base * e, void * data) {
	struct banner * banner = (struct banner *)e  ;
	banner->start_time = timing_begin_to_measure_time();

	switch (e->config_id) {
		case 1:
			banner->config.rotate = *(struct banner_config_rotate*)data;			
			banner_rotate_effect(banner, banner->config.rotate.direction, banner->config.rotate.step);
			break;
		case 2:
			banner->config.blink = *(struct banner_config_blink*)data;						
			banner_blink_effect(banner, banner->config.blink.start_idx, banner->config.blink.end_idx, banner->config.blink.time_on, banner->config.blink.time_off);
			break;
		default:
			break;
	}

}

struct effect_ops   banner_ops=  {
	.render = banner_render,
	.config = banner_config,
};


/**
 * Created  09/25/2023
 * @brief  initilize banner  
 * @note  
 * @param    
 * @return  
 */
void banner_init_with_text(struct banner * banner, struct rect r, const struct font * font,const char* fmt, ...){ 
	va_list args;
	va_start(args, fmt);

	vsnprintf(banner->text[0],sizeof(banner->text), fmt, args);

	banner->font = font;
	banner->effect.r = r;
	banner->effect.ops = &banner_ops;
	banner->x = 0;
	banner->y = 0;

}

struct effect_base * banner_new() {
	struct banner * b = lua_malloc(sizeof(struct banner));
	if (b)
		memset (b,0,sizeof(struct banner));

	b->effect.ops = &banner_ops;
	return &b->effect;
}

void banner_free(struct banner * banner) {
}

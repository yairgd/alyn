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



void banner_set_text(struct banner * banner,const char* fmt, ...){ 
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

	canvas_get_rect(&banner->canvas, r, rect_buffer);

}

static void banner_blink_effect(struct banner * banner, int rate, int start_idx, int end_idx, int tick_time) {
	banner->effect_id = 1;	
	banner->tick = 0;
	banner->config.blink.rate = rate;
	banner->config.blink.start_idx = start_idx;
	banner->config.blink.end_idx = end_idx;
	banner->config.blink.tick_time = tick_time;
	banner->config.blink.on = 1;

}

static void banner_rotate_effect(struct banner * banner, int direction, int step, int tick_time) {
	banner->effect_id = 2;
	banner->tick = 0;	
	banner->config.rotate.direction = direction;
	banner->config.rotate.tick_time = tick_time;
	banner->config.rotate.step = step;

}


static void bannner_manage_blink(struct banner * banner) 
{
	banner->tick++;

	if (banner->tick >= banner->config.blink.rate) {
		banner->tick = 0;
		if (banner->config.blink.on == 1) {
			banner->config.blink.on = 0;
			if (banner->config.blink.end_idx !=0 && banner->config.blink.start_idx <= banner->config.blink.end_idx)
				// blink range of letters
				canvas_fill_rect(&banner->canvas, &(struct rect  ){/*banner->x +*/ banner->config.blink.start_idx * font_width(banner->canvas.font,0) ,0/*banner->y*/,banner->config.blink.end_idx * font_width(banner->canvas.font,0) , font_height(banner->canvas.font)},0);

			else   { // blink all text
				int len = u8_strlen(banner->text);				
				canvas_fill_rect(&banner->canvas, &(struct rect  ){0/*banner->x*/,0/*banner->y*/,len * font_width(banner->canvas.font,0) , font_height(banner->canvas.font)},0);
			}
		}
		else {
			banner->config.blink.on = 1;
			canvas_print(&banner->canvas, 0/*banner->x*/,0 /*banner->y*/, banner->text);
		}
	}
}

static void bannner_manage_rotate(struct banner * banner) {
	banner->tick++;

	if (banner->tick >= banner->config.rotate.tick_time) {
		banner->tick = 0;
		if (banner->config.rotate.direction == 1) {
			canvas_rotate_left(&banner->canvas,banner->config.rotate.step);	
		} else if (banner->config.rotate.direction == 2) {
			canvas_rotate_right(&banner->canvas,banner->config.rotate.step);				
		}
	}

}



static void banner_render(struct effect_base * e,  struct canvas * canvas, struct  rect * r) {
	struct banner * banner = e->object_data;
	static char b[64*32*4];

	switch (banner->effect_id) {
		case 1: // blink
			bannner_manage_blink(banner);
			break;
		case 2: // rotate
			bannner_manage_rotate(banner);
			// copy data from local canvas to display
			canvas_get_rect(&banner->canvas, &(struct rect){0,0 , e->r.width ,  e->r.height   },b);
			canvas_set_rect(canvas, &(struct rect){e->r.top_left_x,e->r.top_left_y , e->r.width ,  e->r.height   },b);
			break;
		default:
			// do nothing
			break;
	};

	


}



static void banner_config(struct effect_base * e) {
	struct effect_configuration * config = &e->config;
	struct banner * banner = e->object_data;
	union banner_config * banner_config =  config->data;
	switch (config->id) {
		case 1:
			banner_rotate_effect(banner, banner_config->rotate.direction, banner_config->rotate.step, banner_config->rotate.tick_time);
			break;
		case 2:
			banner_blink_effect(banner, banner_config->blink.rate, banner_config->blink.start_idx, banner_config->blink.end_idx, banner_config->blink.tick_time);
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

	vsnprintf(banner->text,sizeof(banner->text), fmt, args);
	int len = u8_strlen(banner->text);
	canvas_init(&banner->canvas,len * font_width(font,0) , font_height(font) );
	canvas_set_font(&banner->canvas, font);
	canvas_print(&banner->canvas, 0,0, banner->text);

	banner->effect.r = r;
	banner->effect.ops = &banner_ops;
	banner->effect.object_data = banner;

}

struct effect_base * banner_new() {
	struct banner * b = malloc(sizeof(struct banner));
	if (b)
		memset (b,0,sizeof(struct banner));
	
	b->effect.ops = &banner_ops;
	b->effect.object_data = b;
	return &b->effect;
}

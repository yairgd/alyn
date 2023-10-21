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
#include "timing.h"

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

static void banner_blink_effect(struct banner * banner, int start_idx, int end_idx, double time_on, double time_off) {
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


static void bannner_manage_blink(struct banner * banner) 
{

	if (banner->on == 1 && timing_elapse(&banner->start_time, banner->config.blink.time_on) ) {
		banner->on = 0;
		timing_begin_to_measure_time(&banner->start_time);
		if (banner->config.blink.end_idx !=0 && banner->config.blink.start_idx <= banner->config.blink.end_idx)
			// blink range of letters
			canvas_fill_rect(&banner->canvas, &(struct rect  ){banner->config.blink.start_idx * font_width(banner->canvas.font,0) ,0,banner->config.blink.end_idx * font_width(banner->canvas.font,0) , font_height(banner->canvas.font)},0);

		else   { // blink all text
			int len = u8_strlen(banner->text);				
			canvas_fill_rect(&banner->canvas, &(struct rect  ){0,0,len * font_width(banner->canvas.font,0) , font_height(banner->canvas.font)},0);
		}
	}
	else if (banner->on == 0 && timing_elapse(&banner->start_time, banner->config.blink.time_off) ) {
		banner->on = 1;
		timing_begin_to_measure_time(&banner->start_time);		
		canvas_print(&banner->canvas, 0,0, banner->text);
	}
}

static void bannner_manage_rotate(struct banner * banner) {

	if (banner->config.rotate.direction == 1) {
		canvas_rotate_left(&banner->canvas,banner->config.rotate.step);	
	} else if (banner->config.rotate.direction == 2) {
		canvas_rotate_right(&banner->canvas,banner->config.rotate.step);				
	}

}


static char b[64*32*4];

static void banner_render(struct effect_base * e,  struct canvas * canvas, struct  rect * r) {
	struct banner * banner = (struct banner *)e  ;

	memset (b,0,64*32*4);
	switch (banner->effect.config_id) {
		case 1: // blink
			bannner_manage_blink(banner);
			break;
		case 2: // rotate
			bannner_manage_rotate(banner);
			break;
		default:
			// do nothing
			break;
	};
	// copy data from local canvas to display
	canvas_get_rect(&banner->canvas, &(struct rect){0,0 , e->r.width ,  e->r.height   },b);
	canvas_set_rect(canvas, &e->r,b); // &(struct rect){e->r.top_left_x,e->r.top_left_y , e->r.width ,  e->r.height   },b);
}



static void banner_config(struct effect_base * e, void * data) {
	struct banner * banner = (struct banner *)e  ;
	timing_begin_to_measure_time(&banner->start_time);

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

	vsnprintf(banner->text,sizeof(banner->text), fmt, args);
	int len = u8_strlen(banner->text);
	canvas_init(&banner->canvas,len * font_width(font,0) , font_height(font) );
	canvas_set_font(&banner->canvas, font);
	canvas_print(&banner->canvas, 0,0, banner->text);

	banner->effect.r = r;
	banner->effect.ops = &banner_ops;
}

struct effect_base * banner_new() {
	struct banner * b = malloc(sizeof(struct banner));
	if (b)
		memset (b,0,sizeof(struct banner));

	b->effect.ops = &banner_ops;
	return &b->effect;
}

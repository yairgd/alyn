/*
 * =====================================================================================
 *
 *       Filename:  aniate_rectange.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/30/2023 03:50:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdlib.h>

#include "frame.h"
#include "effect.h"
#include "utils/lua_memory.h"



static inline int R(int x, int m) {
	int _idx = x % m;
	_idx += (_idx < 0) ? m : 0;
	return _idx;
}

/**
 * Created  07/17/24
 * @brief   draw frame around rectngle
 * @param   
 * @return  
 */
static void _draw_points_of_rectangle_frame(struct canvas * canvas, struct frame * f) {
	int c1 = f->config.animate_frame.c1;
	int c2 = f->config.animate_frame.c2;
	int rate = f->config.animate_frame.color_change_rate;
	struct rect * r = &f->config.animate_frame.r;
	int dir = f->config.animate_frame.dir;
	int idx = 0;
	int size = r->height * 2 + r->width*2 - 4;

	for (int x = r->top_left_x; x < r->top_left_x + r->width ;x++) {
		canvas_plot(canvas, x,  r->top_left_y, f->points[R(idx + f->cnt ,size)].c);
		idx++;
	}

	for (int y = r->top_left_y+1; y < r->top_left_y + r->height-1;y++) {
		canvas_plot(canvas,  r->top_left_x + r->width - 1, y, f->points[ R(idx + f->cnt , size)].c);
		idx++;
	}

	for (int x = r->top_left_x + r->width-1 ; x >= r->top_left_x; x--) {
		canvas_plot(canvas, x, r->top_left_y + r->height - 1, f->points[R(idx + f->cnt , size)].c);
		idx++;
	}

	for (int y = r->top_left_y + r->height - 2  ; y  > r->top_left_y; y--) {
		canvas_plot(canvas, r->top_left_x, y,  f->points[R(idx + f->cnt ,size)].c);		
		idx++;
	}

		
	if (f->speed) 
		 f->speed--;
	 else {
		 f->speed = dir > 0 ? dir : -dir;
		 if (dir < 0)
			 f->cnt++;
		 else if (dir > 0) {
			 f->cnt--;
			 if (f->cnt<0) 
				 f->cnt =  r->height * 2 + r->width*2 - 4 - 1;
		 }
	 }

}

/**
 * Created  09/30/2023
 * @brief   initilize the frame to be in rectange shape
 * @note  
 * @param   
 * @return  
 */
static void type_1_animate_frame_init(struct frame * f ) 
{
	int c1 = f->config.animate_frame.c1;
	int c2 = f->config.animate_frame.c2;
	int rate = f->config.animate_frame.color_change_rate;
	struct rect * r = &f->config.animate_frame.r;

	int s = 0;
	for (int i = 0; i < r->height * 2 + r->width*2 - 4;i++) {
		if (i % rate == 0)
			s=(s==0) ? 1 : 0;
		f->points[i].c = (s) ? PIXEL(c1) : PIXEL(c2);
	}

	f->cnt = 0;
}

static void _render_animate_frame(struct frame * f, struct canvas * canvas) {
	_draw_points_of_rectangle_frame(canvas , f);		 
	 	
}

static void frame_render(struct effect_base * e ,  struct canvas * canvas, struct  rect * r) {

	struct frame * f = (struct frame *)e; // e->object_data;

	f->effect.canvas = canvas;
	switch (e->config_id) {
		case 1:
			_render_animate_frame(f, canvas);
			break;
		default:
			break;
	}

}


static void frame_config(struct effect_base * e, void * data) {
	struct frame * frame = (struct frame *)e; 

	switch (e->config_id) {
		case 1:
			frame->config.animate_frame = *(struct animate_frame*)data;			
			type_1_animate_frame_init(frame) ;
			break;
		default:
			break;
	}

}

struct effect_ops   frame_ops=  {
	.render = frame_render,
	.config = frame_config,
};


void frame_init(struct frame  * f) {
	f->effect.ops = &frame_ops;
}


struct effect_base * frame_new() {
	struct frame * f = (void*)lua_malloc( sizeof(struct frame));
	if (f)
		memset (f,0,sizeof(struct frame));

	f->effect.ops = &frame_ops;
	return &f->effect;
}

void frame_free(struct frame  * f) {
	lua_free(f);
}


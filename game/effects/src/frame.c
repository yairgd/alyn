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


static inline int _color_switch(int c1,int c2, int rate) {
	static int cnt = 0;
	static int s =0;

	cnt++;
	if (cnt%rate == 0)
		s=(s==0) ? 1 : 0;
	return (s) ? c1 : c2;
}


static inline void _color_shift_inside_group (struct frame * f, int dir, int pixel_change) {
	for (int i = 0; i < pixel_change; i++) {
		for (int g = 0; g < f->num_of_groups; g++) {	
			// volor shift inside group
			if (dir) {
				// color shift left				
				int c = f->points[f->group[g].start_idx].c;
				for (int i = f->group[g].start_idx + 1; i <=  f->group[g].end_idx;i++) {
					f->points[i-1].c = f->points[i].c;
				}
				f->points[f->group[g].end_idx].c = c;
			} else {
				// color shift right
				int c = f->points[f->group[g].end_idx].c;
				for (int i = f->group[g].end_idx; i >  f->group[g].start_idx;i--) {
					f->points[i].c = f->points[i-1].c;					
				}			
				f->points[f->group[g].start_idx].c = c;
			}
		}
	}
}


/**
 * Created  10/01/2023
 * @brief   plots the pixels of the fram
 * @note  
 * @param   type the plotting type:0 - each call, plot sub group of pixels from one of the  pixel groups, 1 plot all pixels
 * @return  
 */
static void _plot_pixels(struct frame * f, int type) {
	for (int g = 0; g < f->num_of_groups; g++) {
		if (type) {
			// plot one point from ecah group
			if (f->group[0].end_idx == f->group[g].current_idx) {
				f->group[g].current_idx = f->group[g].start_idx;
			}
			canvas_plot(f->effect.canvas, f->points[f->group[g].current_idx].x, f->points[f->group[g].current_idx].y, f->points[f->group[g].current_idx].c);
			f->group[g].current_idx++;
		} else {

			// plot all points
			for (int i = f->group[g].start_idx ; i <=  f->group[g].end_idx;i++) {
				canvas_plot(f->effect.canvas, f->points[i].x, f->points[i].y, f->points[i].c);					
			}
		}	
	}
}



/**
 * Created  10/01/2023
 * @brief   collects points of rectangle frame
 * @note  
 * @param   
 * @return  
 */
static void _collects_points_of_rectangle_frame(struct frame * f) {
	int idx = 0;
	int c1 = f->config.animate_frame.c1;
	int c2 = f->config.animate_frame.c2;
	int rate = f->config.animate_frame.color_change_rate;
	struct rect * r = &f->config.animate_frame.r;

	// take all point from upper line -->
	for (int x = r->top_left_x; x < r->top_left_x + r->width ;x++) {
		f->points[idx].c = _color_switch(c1, c2,rate);
		f->points[idx].x = x;
		f->points[idx++].y = r->top_left_y;

	}

	for (int y = r->top_left_y; y < r->top_left_y + r->height;y++) {
		f->points[idx].c = _color_switch(c1, c2,rate) ;
		f->points[idx].x = r->top_left_x + r->width - 1 ;
		f->points[idx++].y = y;

	}

	for (int x = r->top_left_x + r->width - 1; x >= r->top_left_x; x--) {
		f->points[idx].c = _color_switch(c1, c2,rate) ;	
		f->points[idx].x = x;
		f->points[idx++].y = r->top_left_y + r->height - 1;

	}

	for (int y = r->top_left_y + r->height - 1; y > r->top_left_y; y--) {
		f->points[idx].c = _color_switch(c1, c2,rate) ;	
		f->points[idx].x = r->top_left_x;
		f->points[idx++].y = y;
	}

	f->num_of_points = idx;

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
	_collects_points_of_rectangle_frame(f);

	f->num_of_groups = 1;
	f->group[0].start_idx = 0;
	f->group[0].end_idx = f->num_of_points - 1;
	f->group[0].current_idx = 0;
}

static void _render_animate_frame(struct frame * f) {
	_plot_pixels(f,0);
	_color_shift_inside_group(f, f->config.animate_frame.dir,f->config.animate_frame.pixel_change);

}

static void frame_render(struct effect_base * e ,  struct canvas * canvas, struct  rect * r) {

	struct frame * f = (struct frame *)e; // e->object_data;

	f->effect.canvas = canvas;
	switch (e->config_id) {
		case 1:
			_render_animate_frame(f);
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
	struct frame * f = malloc(sizeof(struct frame));
	if (f)
		memset (f,0,sizeof(struct frame));

	f->effect.ops = &frame_ops;
	return &f->effect;
}

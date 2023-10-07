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

#include "frame.h"


static inline int _color_switch(int c1,int c2, int rate) {
	static int cnt = 0;
	static int s =0;

	cnt++;
	if (cnt%rate == 0)
		s=(s==0) ? 1 : 0;
	return (s) ? c1 : c2;
}


static inline void _color_shift_inside_group (struct frame * f, int dir) {
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
				canvas_plot(f->canvas, f->points[f->group[g].current_idx].x, f->points[f->group[g].current_idx].y, f->points[f->group[g].current_idx].c);
				f->group[g].current_idx++;
			} else {

				// plot all points
				for (int i = f->group[g].start_idx ; i <=  f->group[g].end_idx;i++) {
					canvas_plot(f->canvas, f->points[i].x, f->points[i].y, f->points[i].c);					
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
	int c1 = f->effect.animate_frame.c1;
	int c2 = f->effect.animate_frame.c2;
	int rate = f->effect.animate_frame.color_change_rate;
	struct rect * r = &f->effect.animate_frame.r;

	// take all point from upper line -->
	for (int x = r->top_left_x; x < r->top_left_x + r->width ;x++) {
		f->points[idx].c = _color_switch(c1, c2,rate);
		f->points[idx].x = x;
		f->points[idx++].y = r->top_left_y;

	}

	for (int y = r->top_left_y; y < r->top_left_y + r->height;y++) {
		f->points[idx].c = _color_switch(c1, c2,rate) ;
		f->points[idx].x = r->top_left_x + r->width ;
		f->points[idx++].y = y;

	}

	for (int x = r->top_left_x + r->width - 1; x > r->top_left_x; x--) {
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
void frame_rectangle_init(struct frame * f , struct canvas * canvas,struct animate_config_frame * af ) 
{
	f->canvas = canvas;

	f->type = 1;
	f->effect.animate_frame = *af;
	_collects_points_of_rectangle_frame(f);


	f->num_of_groups = 1;
	f->group[0].start_idx = 0;
	f->group[0].end_idx = f->num_of_points - 1;
	f->group[0].current_idx = 0;
}


static void frame_animate_rectangle_frame_manage(struct frame * f) {
	f->tick++;
	if (f->tick == f->effect.animate_frame.tick) {
		f->tick=0;
		_plot_pixels(f,0);
		_color_shift_inside_group(f, 1);

	}
}

void frame_manage(struct frame * f) {

	switch (f->type) {
		case 1:
			frame_animate_rectangle_frame_manage(f);
			break;
		default:
			break;
	}
	
}

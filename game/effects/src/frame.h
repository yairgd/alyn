/*
 * =====================================================================================
 *
 *       Filename:  aniate_rectange.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/30/2023 03:50:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef ANIMATE_RECTANGLE_H
#define ANIMATE_RECTANGLE_H 
#include "effect.h"

#define MAX_POINTS_PER_FRAME 32*64
struct frame {
	struct effect_base  effect;
	struct point {
		unsigned int x : 16;
		unsigned int y : 16;
		unsigned int c ;
	} points[MAX_POINTS_PER_FRAME];
	int num_of_points;

	struct points_group {
		unsigned short start_idx : 16;
		unsigned short end_idx : 16;
		unsigned short current_idx : 16;
	} group[10];
	int num_of_groups;
	int tick;
	
	int type; // 1: two colors rotate frame 
	union  {
		//  two colors rotate frame 
		struct animate_frame {
			int c1;
			int c2;
			int color_change_rate;
			int tick;
			struct rect  r;
		} animate_frame;
	} config;
};

#endif

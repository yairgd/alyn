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

#ifdef __cplusplus
extern "C" {
#endif


#include "effect.h"

#define MAX_POINTS_PER_FRAME (64+64+32+32)

	struct animate_frame {
		int c1;
		int c2;
		int color_change_rate;
		int pixel_change;   // how many pixel to move in every render call
		int dir;
		struct rect  r;
	} ;

	struct frame {
		struct effect_base  effect;
		struct point {
			struct pixel c ;
		} points[MAX_POINTS_PER_FRAME];


		int cnt;	
		int speed;
		union  {
			//  two colors rotate frame 
			struct animate_frame animate_frame;
		} config;
	};

	struct effect_base * frame_new();
	void frame_init(struct frame  * f);
	void frame_free(struct frame  * f);




	#define FRAME_CONFIG_TYPE_1(c1,c2,color_change_rate,pixel_change,dir,r) \
		&(struct effect_configuration){1, &(struct animate_frame){ (c1), (c2), (color_change_rate) ,(pixel_change),(dir), (r)  } }



#ifdef __cplusplus
}
#endif

#endif

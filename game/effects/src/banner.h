/*
 * =====================================================================================
 *
 *       Filename:  banner.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2023 08:01:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef BANNER_H
#define BANNER_H 

#ifdef __cplusplus
extern "C" {
#endif

#include "canvas.h"
#include "effect.h"

	struct banner_config_rotate {
				int tick_time;
				int step;
				int direction; //0 no rotation, 1 left 2, right
					       //int cnt;
			} ;

	/**
	 * @class banner
	 * @brief This class represents a banner to be displayed on the screen. Its primary function
	 * is to manage the Canvas class for rendering the banner.
	 */
	struct banner {
		struct effect_base  effect;

		struct canvas canvas;
		char text[256];
		int tick;
		int effect_id;  // 0, none , 1 blink, 2
		union banner_config {
			struct banner_config_blink {
				int rate ;     // is abpove 0, the letters blink, 
				int start_idx; // the index of letter to blink, if -1, all letter blink
				int end_idx;
				int on;  // 0 - no text, 1 display text
				int tick_time;
				//int cnt ;
			} blink;
			struct banner_config_rotate rotate;
		} config;
	};



	//(struct effect_configuration){.id = 1, &(struct banner_config_rotate){.tick_time = (_tick_time), .step =(_step), .direction = (_direction)   } }

#define BANNER_CONFIG_BLINK(_rate,_start_idx,_end_idx,_on,_tick_time) \
	(struct effect_configuration){.id = 2, (struct banner_config_blink){.rate = (_rate), .start_idx =(_start_idx), .end_idx = (_end_idx),.on=(_on),.tick_time=(_tick_time)} }

//#define BANNER_CONFIG_ROTATE(_tick_time,_step,_direction) \
//	(struct effect_configuration){1, &(struct banner_config_rotate){ (_tick_time), (_step), (_direction)   } }


	static inline struct effect_configuration  BANNER_CONFIG_ROTATE(int tick_time, int step, int direction) {
		struct effect_configuration ec;
		struct banner_config_rotate bc = {tick_time,step,direction};	
		ec.id = 1; 
		ec.data = &bc;
		return ec;
	}




	void banner_init_by_canvas(struct banner * banner, void (*do_effect)(struct canvas *));
	struct canvas * banner_get_canvas(struct banner * banner);
	void banner_set_text(struct banner * banner,const char* fmt, ...);
	void banner_get_buffer(struct banner * banner, struct rect * r,  char *rect_buffer);
	void banner_init_with_text(struct banner * banner, struct rect  r, const struct font * font,const char* fmt, ...);
	struct effect_base * banner_new() ;
#ifdef __cplusplus
}
#endif

#endif
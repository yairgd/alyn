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
#include "canvas.h"



/**
 * @class banner
 * @brief This class represents a banner to be displayed on the screen. Its primary function
 * is to manage the Canvas class for rendering the banner.
 */
struct banner {
	int is_init;
	struct canvas canvas;
	char text[256];
	int tick;
	int effect_id;  // 0, none , 1 blink, 2
	union effect {
		struct blink_effect {
			int rate ;     // is abpove 0, the letters blink, 
			int start_idx; // the index of letter to blink, if -1, all letter blink
			int end_idx;
			int on;  // 0 - no text, 1 display text
			int tick_time;
			//int cnt ;
		} blink;
		struct rotate_effect {
			int tick_time;
			int step;
			int direction; //0 no rotation, 1 left 2, right
			//int cnt;
		} rotate;
		struct canvas_effet {
			// can use to manipulate canvas using external function
			void (*do_effect)(struct canvas *);
		} canvas;
	} effet;
};

void banner_init_by_canvas(struct banner * banner, void (*do_effect)(struct canvas *));
struct canvas * banner_get_canvas(struct banner * banner);
void banner_set_text(struct banner * banner, int x, int y,const char* fmt, ...);
void banner_get_buffer(struct banner * banner, struct rect * r,  char *rect_buffer);
void banner_init_with_text(struct banner * banner, const struct font * font, int x, int y,const char* fmt, ...);
void banner_blink_effect(struct banner * banner, int rate, int start_idx,int end_idx, int tick_time);
void banner_rotate_effect(struct banner * banner, int direction, int step, int tick_time);
void banner_manage (struct banner * banner);
int banner_is_initialize (struct banner * banner);
char * banner_get_canvas_buffer(struct banner * banner);
int banner_width (struct banner * banner);
int banner_height (struct banner * banner);

#endif

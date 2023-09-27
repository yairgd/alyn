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


struct rect {
		int top_left_x;
		int top_left_y;
		int buttom_right_x;
		int buttom_right_y;
	} ;


/**
 * @class banner
 * @brief This class represents a banner to be displayed on the screen. Its primary function
 * is to manage the Canvas class for rendering the banner.
 */
struct banner {
	struct canvas canvas;
	char * rect_buffer;
	int blink_rate ;     // is abpove 0, the letters blink, 
	int letter_blink_id; // the index of letter to blink, if -1, all letter blink
	char text[256];

};
void banner_init(struct banner * banner, int width, int height);
void banner_set_text(struct banner * banner, int x, int y,const char* fmt, ...);
void banner_set_rect(struct banner * banner, int tlx, int tly , int brx, int bry);
const char * banner_get_buffer(struct banner * banner, struct rect * r);
void banner_init_with_text(struct banner * banner, int x, int y,const char* fmt, ...);

int rect_width(struct rect * rect);
int rect_height(struct rect * rect);

#endif

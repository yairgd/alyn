/*
 * =====================================================================================
 *
 *       Filename:  rect.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/12/2024 02:56:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef RECT_H
#define RECT_H


struct rect {
	int top_left_x;
	int top_left_y;
	int width;
	int height;
} ;

int rect_width(struct rect * rect);
int rect_height(struct rect * rect);

#define RECT(x,y,w,h) \
	(struct rect){(x),(y),(w),(h)}
#endif

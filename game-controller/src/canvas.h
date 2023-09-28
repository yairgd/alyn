/*
 * =====================================================================================
 *
 *       Filename:  canvas.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/22/2023 10:55:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef CANVAS_H
#define CANVAS_H 
#include "font.h"


#define GET_RED(x)   ((x) & 0xff0000)>>16)
#define GET_GREEN(x) ((x) & 0x00ff00)>> 8)
#define GET_BLUE(x)  ((x) & 0x0000ff)>> 0)
#define RGB(r,g,b)  ((r) << 16 | (g) << 8 | (b) << 0)
#define SET_BIT_COLOR(canvas,w,h,c)(*((int*)&canvas->buffer[ + 4 * (w) + 4 * (h) * canvas->width]) = (c))
#define GET_BIT_COLOR(canvas,w,h) (*((int*)&canvas->buffer[4 * (w) + 4 * (h) * canvas->width]))
#define bit(x,n)(x[(n)/8] & (1 << (7-(n)%8)) ? 1 : 0)

struct rect {
	int top_left_x;
	int top_left_y;
	int buttom_right_x;
	int buttom_right_y;
} ;

int rect_width(struct rect * rect);
int rect_height(struct rect * rect);

struct canvas {
	const struct font * font;
	char *buffer;
	int font_color;
	int bg_color;


	int width;
	int height;
};

void canvas_init(struct canvas * canvs, const struct font * f, int width, int height);
void canvas_write_font(struct canvas * canvas, int char_id, int x,int y);
void canvas_dump(struct canvas * canvas);
const char * canvas_buffer(struct canvas * canvas);
void canvas_rotate_right(struct canvas * canvas, int n);
void canvas_rotate_left(struct canvas * canvas, int n);
void canvas_free(struct canvas * canvas);
void canvas_print(struct canvas * canvas, int x, int y,const char* txt);
void canvas_fill_rect(const struct canvas * canvas, struct rect * rect,int color);


#endif

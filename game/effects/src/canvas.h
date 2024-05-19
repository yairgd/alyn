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
#include <stdint.h>

#define GET_RED(x)   ((x) & 0xff0000)>>16)
#define GET_GREEN(x) ((x) & 0x00ff00)>> 8)
#define GET_BLUE(x)  ((x) & 0x0000ff)>> 0)
#define RGB(r,g,b)  ((r) << 16 | (g) << 8 | (b) << 0)
//#define SET_BIT_COLOR(canvas,w,h,c)(*((int*)&canvas->buffer[ + 4 * (w) + 4 * (h) * canvas->width]) = (c))
//#define GET_BIT_COLOR(canvas,w,h) (*((int*)&canvas->buffer[4 * (w) + 4 * (h) * canvas->width]))
#define bit(x,n)(x[(n)/8] & (1 << (7-(n)%8)) ? 1 : 0)
#define SET_PIXEL_BIT(buffer,n,v)(buffer[(n)/8] |= (v << (7-(n)%8)))


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

struct canvas {
	const struct font * font;
	char *buffer;
	int font_color;
	int bg_color;


	int width;
	int height;
};

static inline uint32_t  GET_BIT_COLOR(struct canvas * c,int w,int h) {
	return (*((int*)&c->buffer[4 * (w) + 4 * (h) * c->width]));
}

static inline uint32_t * GET_POINTER_TO_PIXEL(struct canvas * c,int w,int h) {
	return ((uint32_t*)&c->buffer[4 * (w) + 4 * (h) * c->width]);
}

static inline void  SET_BIT_COLOR(struct canvas * canvas,int w,int h, int c) {
	*((int*)&canvas->buffer[ + 4 * (w) + 4 * (h) * canvas->width]) = c;	
}

void canvas_init(struct canvas * canvs, int width, int height);
void canvas_write_font(struct canvas * canvas, int char_id, int x,int y);
void canvas_dump(struct canvas * canvas);
const char * canvas_buffer(struct canvas * canvas);
void canvas_rotate_right(struct canvas * canvas, int n);
void canvas_rotate_left(struct canvas * canvas, int n);
void canvas_free(struct canvas * canvas);
void canvas_print(struct canvas * canvas, int x, int y,const char* txt);
void canvas_set_font(struct canvas * canvs, const struct font * f);
void canvas_get_rect(struct canvas * canvas, struct rect * r, char *rect_buffer);
void canvas_set_rect(struct canvas * canvas, struct rect * r, char *rect_buffer) ;
void canvas_fill_rect( struct canvas * canvas, struct rect * rect,int color);
void canvas_plot( struct canvas * canvas, int x,int y,int color);
void canvas_line( struct canvas * canvas, int x1,int y1,int x2, int y2,int c);
void canvas_circle( struct canvas * canvas, int x,int y,int r,int c);
void canvas_fill_circle( struct canvas * canvas, int x0,int y0,int radius,int c) ;
void canvas_clean( struct canvas * canvas) ;
void canvas_clean_rect( struct canvas * canvas, struct rect *r);
#endif

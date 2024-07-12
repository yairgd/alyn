/*
			return static_cast<uint8_t>(GetHalMgr().PerformMemoryBadBlocksTest());
 * =====================================================================================
 *
 *       Filename:  canvas.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/22/2023 10:55:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "canvas.h"
#include "u8.h"

static struct rect g_rect; 

int rect_width(struct rect * r) {
	return r->width;

}
int rect_height(struct rect * r){
	return r->height;
}


void canvas_init(struct canvas * canvas, int width, int height, struct pixel * pixel)
{
	memset (canvas, 0, sizeof(*canvas));
	canvas->font = 0;
	canvas->font_color = RGB(255,255,255);
	canvas->bg_color = RGB(0,0,0);


	canvas->width = width;
	canvas->height = height;
	canvas->buffer = pixel;


	memset (canvas->buffer,0, PIXEL_SIZE * width * height); 

	g_rect.top_left_x = 0;
	g_rect.top_left_y = 0;
	g_rect.height = canvas->height;
	g_rect.width = canvas->width;
	canvas_set_global_rect(canvas);	
}




void canvas_free(struct canvas * canvas)
{
	memset (canvas, 0, sizeof (*canvas));	
}

void canvas_write_font(struct canvas * canvas, int char_id, int x,int y) {
	const char * buf = font_get_map(canvas->font, char_id);

	for (int h = 0; h < canvas->font->Height /* && y + h < canvas->height*/; h++) {
		for (int w = 0; w < canvas->font->Width /* && x + w < canvas->width*/; w++) {
			if (bit(buf, w)) {
				canvas_plot(canvas, x+w,y+h,canvas->font_color) ;
				//	printf("x");				
			} else {
				canvas_plot(canvas, x+w,y+h,canvas->bg_color); 				
				//printf("_");				
			}

		}
		buf += font_chars_per_row(canvas->font);
		//printf("\n");		
		//
	}
}

void canvas_dump(struct canvas * canvas) {
	for (int h = 0; h < canvas->height; h++) {
		for (int w = 0; w < canvas->width; w++) {
			if (*(uint32_t*)GET_POINTER_TO_PIXEL(canvas,w,h)) {
				printf("x");
			} else {
				printf("_");
			}
		}
		printf("\n");		
	}
}

const struct pixel * canvas_buffer(struct canvas * canvas) {
	return canvas->buffer;
}

static char c[20*4]; 
void canvas_rotate_right(struct canvas * canvas, int n) {
	if (n > 20)
		n = 19;
	for (int h = 0; h < canvas->height; h++) {
		memcpy (c, &canvas->buffer[(canvas->width  * h)],n * PIXEL_SIZE);
		for (int i = 0; i < (canvas->width - n); i++)
			canvas->buffer[(canvas->width * h  + i )] = canvas->buffer[canvas->width *  h +   n + i];
		memcpy (&canvas->buffer[(canvas->width * h  + canvas->width  -    n)] , c,n * PIXEL_SIZE);
	}
}

void canvas_rotate_left(struct canvas * canvas, int n) {
	if (n > 20)
		n = 19;
	for (int h = 0; h < canvas->height; h++) {
		memcpy (c, &canvas->buffer[(canvas->width  * h +   (canvas->width - 2*n))],n * PIXEL_SIZE);
		for (int i = (canvas->width ); i >   n ; i--)
			canvas->buffer[canvas->width  * h+i-n ] =canvas->buffer[canvas->width  * h+i-2*n ];
		memcpy (&canvas->buffer[(canvas->width  * h )] , c,n * PIXEL_SIZE);
	}
}

void canvas_print(struct canvas * canvas, int x, int y, const char* str){ 
	int len, unicode;
	int i = 0;
	if  (canvas->font == 0)
		canvas->font = font_6x13();

	while (*str) {
		len = u8_to_unicode(str, &unicode );

		int char_value = UNICODE_TO_ISO8859_8(unicode);

		canvas_write_font(canvas,char_value,x+i*font_width(canvas->font,0) ,y);		
		i++;
		//printf("%x\n", char_value);
		str += len;		
	}


}


void canvas_fill_rect( struct canvas * canvas, struct rect * r,int color) {
	int buttom_right_y = r->top_left_y + r->height - 0;
	int buttom_right_x = r->top_left_x + r->width - 0;

	for (int x = r->top_left_x; x < buttom_right_x && x < canvas->width; x++) {
		for (int y = r->top_left_y; y < buttom_right_y && y < canvas->height; y++) {
			canvas_plot(canvas, x, y,PIXEL(color));			
		}
	}
}

void canvas_set_font(struct canvas * canvas, const struct font * f)
{
	canvas->font = f;
}


/**
 * Created  09/26/2023
 * @brief   return data from rectangle. The function
 * rearrange the new rectangle.
 * @note  
 * @param   
 * @return  
 */
void canvas_get_rect(struct canvas * canvas, struct rect * r, char *rect_buffer) {
	int buttom_right_x = r->top_left_x + r->width - 0;
	int buttom_right_y = r->top_left_y + r->height - 0;

	for (int y = r->top_left_y; y < buttom_right_y && y < canvas->height; y ++) {
		for (int x = r->top_left_x; x < buttom_right_x && x < canvas->width; x++) {
			int pixel = *(int*)&canvas->buffer[ (x) +   (y) * canvas->width];
			*(int*)&rect_buffer[ (x - r->top_left_x) +   (y - r->top_left_y) *  r->width] = pixel;
		}
	}
}

/**
 * Created  09/26/2023
 * @brief   place data in rectange area (for example, to copy pre made icons) 
 * @note  
 * @param   
 * @return  
 */
void canvas_copy_rect(struct canvas * canvas, struct rect * r, char *rect_buffer) {
	int buttom_right_x = r->top_left_x + r->width - 0;
	int buttom_right_y = r->top_left_y + r->height - 0;

	for (int y = r->top_left_y; y < buttom_right_y && y < canvas->height; y ++) {
		for (int x = r->top_left_x; x < buttom_right_x && x < canvas->width; x++) {
			int pixel = *(int*)&rect_buffer[(x - r->top_left_x) +  (y - r->top_left_y) *  r->width] ;
			*(int*)&canvas->buffer[ (x) +  (y) * canvas->width] = pixel;
		}
	}
}

void canvas_plot( struct canvas * canvas, int x,int y,struct pixel c) {

	if (canvas->out_rect) {
		if (x < canvas->out_rect->top_left_x) {
			x= (x % canvas->out_rect->width) + canvas->out_rect->width;
		}
		if (x > canvas->out_rect->top_left_x + canvas->out_rect->width) {
			x= x % canvas->out_rect->width;
		}

	}


	int new_x = x + (g_rect.top_left_x + canvas->r->top_left_x);
	int new_y = y + (g_rect.top_left_y + canvas->r->top_left_y);


	if (new_x < canvas->width && new_y < canvas->height && new_x >=0 && new_y>=0 && x<canvas->r->width &&  x<canvas->r->width )
		SET_BIT_COLOR(canvas,new_x,new_y,&c);
}



/**
 * Created  10/09/2023
 * @brief   plot line from point 1 to point 2
 * @note  
 * @param   (x1,y1) point 1
 * @param   (x2,y2) point 2
 * @param   c color (rgb - 24 bit)
 * @return  
 */
void canvas_line( struct canvas * canvas, int x1,int y1,int x2, int y2,int c) {
	float dx = x2 - x1;
	float dy = y2 - y1;

	float length = sqrtf(dx * dx + dy*dy);
	float angle = atan2(dy,dx);

	for (int i = 0; i < length; i++) {
		canvas_plot(canvas, ceil(x1+cos(angle) * i),ceil(y1+sin(angle) * i),PIXEL(c));
	}

}



/**
 * Created  10/09/2023
 * @brief   draw circle
 * https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
 * reffer here: https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
 * alternative: https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/lib/graphics.cc#L120  - aletrnative
 * @note  
 * @param   
 * @return  
 */
void canvas_circle( struct canvas * canvas, int x0,int y0,int radius,int c) {
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = x0 - x; i <= x0 + x; i++)
		{
			if (i== x0 - x || i==x0 + x) {
				canvas_plot(canvas,i, y0 + y,PIXEL(c));
				canvas_plot(canvas,i, y0 - y,PIXEL(c));
			}
		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			if (i==x0 - y || i==x0 + y) {
				canvas_plot(canvas,i, y0 + x,PIXEL(c));
				canvas_plot(canvas,i, y0 - x,PIXEL(c));
			}
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
}

void canvas_fill_circle( struct canvas * canvas, int x0,int y0,int radius,int c) {
	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = x0 - x; i <= x0 + x; i++)
		{
			canvas_plot(canvas,i, y0 + y,PIXEL(c));
			canvas_plot(canvas,i, y0 - y,PIXEL(c));

		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			canvas_plot(canvas,i, y0 + x,PIXEL(c));
			canvas_plot(canvas,i, y0 - x,PIXEL(c));
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}

}


void canvas_clean_rect( struct canvas * canvas, struct rect *r) {

	for (int x = r->top_left_x; x < r->top_left_x + r->width  && x < canvas->width;x++) {
		for (int y = r->top_left_y; y < r->top_left_y + r->width  && y < canvas->height;y++) {
			canvas_plot(canvas, x, y,PIXEL(0));						
		}
	}
}


void canvas_clean( struct canvas * canvas) {
	for (int x = 0; x < canvas->width;x++) {
		for (int y = 0; y < canvas->height;y++) {
			canvas_plot(canvas, x, y,PIXEL(0));						
		}
	}

}


void canvas_set_rect( struct canvas * canvas, struct rect *r, struct rect * out_rect)
{
	canvas->r = r;
	canvas->out_rect = out_rect;
}

void canvas_set_global_rect( struct canvas * canvas)
{
	canvas->r = &g_rect;
	canvas->out_rect = 0;
}


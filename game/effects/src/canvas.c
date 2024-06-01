/*
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
extern void *k_malloc(size_t size);


int rect_width(struct rect * r) {
	return r->width;

}
int rect_height(struct rect * r){
	return r->height;
}


void canvas_init(struct canvas * canvas, int width, int height)
{
	memset (canvas, 0, sizeof(*canvas));
	canvas->font = 0;
	canvas->font_color = RGB(255,255,255);
	canvas->bg_color = RGB(0,0,8);


	canvas->width = width;
	canvas->height = height;
#ifdef ZEPHYR
	canvas->buffer = k_malloc(4 * width * height);
#else
	canvas->buffer = malloc(4 * width * height);
#endif

	memset (canvas->buffer,0,4 * width * height); 
}

void canvas_free(struct canvas * canvas)
{
	if (canvas->buffer) {
		free(canvas->buffer);
		memset (canvas, 0, sizeof (*canvas));
	}
}
void canvas_write_font(struct canvas * canvas, int char_id, int x,int y) {
	const char * buf = font_get_map(canvas->font, char_id);

	for (int h = 0; h < canvas->font->Height && y + h < canvas->height; h++) {
		for (int w = 0; w < canvas->font->Width && x + w < canvas->width; w++) {
			if (bit(buf, w)) {
				SET_BIT_COLOR(canvas,x+w,y+h,canvas->font_color);
				//	printf("x");				
			} else {
				SET_BIT_COLOR(canvas,x+w,y+h,canvas->bg_color);
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
			if (GET_BIT_COLOR(canvas,w,h)) {
				printf("x");
			} else {
				printf("_");
			}
		}
		printf("\n");		
	}
}

const char * canvas_buffer(struct canvas * canvas) {
	return canvas->buffer;
}


static char c[20*4]; 
void canvas_rotate_right(struct canvas * canvas, int n) {
	if (n > 20)
		n = 19;
	for (int h = 0; h < canvas->height; h++) {
		memcpy (c, &canvas->buffer[(canvas->width * 4 * h)],4*n);
		for (int i = 0; i < (canvas->width - n)*4; i++)
			canvas->buffer[(canvas->width * h * 4 + i )] = canvas->buffer[canvas->width * 4 * h + 4 * n + i];
		memcpy (&canvas->buffer[(canvas->width * h * 4 + canvas->width * 4 -  4 * n)] , c,4*n);
	}
}
void canvas_rotate_left(struct canvas * canvas, int n) {
	if (n > 20)
		n = 19;
	for (int h = 0; h < canvas->height; h++) {
		memcpy (c, &canvas->buffer[(canvas->width * 4 * h + 4 * (canvas->width - 2*n*4))],4*n);
		for (int i = (canvas->width )*4; i > 4 * n ; i--)
			canvas->buffer[canvas->width * 4 * h+i-4*n ] =canvas->buffer[canvas->width * 4 * h+i-2*n*4 ];
		memcpy (&canvas->buffer[(canvas->width * 4 * h )] , c,4*n);
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
			SET_BIT_COLOR(canvas,x,y,color);	
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
			int pixel = *(int*)&canvas->buffer[4 * (x) + 4 * (y) * canvas->width];
			*(int*)&rect_buffer[4 * (x - r->top_left_x) + 4 * (y - r->top_left_y) *  r->width] = pixel;
		}
	}
}


/**
 * Created  09/26/2023
 * @brief   place data in tectange
 * @note  
 * @param   
 * @return  
 */
void canvas_set_rect(struct canvas * canvas, struct rect * r, char *rect_buffer) {
	int buttom_right_x = r->top_left_x + r->width - 0;
	int buttom_right_y = r->top_left_y + r->height - 0;

	for (int y = r->top_left_y; y < buttom_right_y && y < canvas->height; y ++) {
		for (int x = r->top_left_x; x < buttom_right_x && x < canvas->width; x++) {
			int pixel = *(int*)&rect_buffer[4 * (x - r->top_left_x) + 4 * (y - r->top_left_y) *  r->width] ;
			*(int*)&canvas->buffer[4 * (x) + 4 * (y) * canvas->width] = pixel;
		}
	}
}


void canvas_plot( struct canvas * canvas, int x,int y,int c) {
	if (x < canvas->width && y < canvas->height && x >=0 & y>=0)
		SET_BIT_COLOR(canvas,x,y,c);
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
		canvas_plot(canvas, ceil(x1+cos(angle) * i),ceil(y1+sin(angle) * i),c);
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
				canvas_plot(canvas,i, y0 + y,c);
				canvas_plot(canvas,i, y0 - y,c);
			}
		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			if (i==x0 - y || i==x0 + y) {
				canvas_plot(canvas,i, y0 + x,c);
				canvas_plot(canvas,i, y0 - x,c);
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
			canvas_plot(canvas,i, y0 + y,c);
			canvas_plot(canvas,i, y0 - y,c);

		}
		for (int i = x0 - y; i <= x0 + y; i++)
		{
			canvas_plot(canvas,i, y0 + x,c);
			canvas_plot(canvas,i, y0 - x,c);
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
			SET_BIT_COLOR(canvas,x,y,0);			
		}
	}
}


void canvas_clean( struct canvas * canvas) {
	for (int x = 0; x < canvas->width;x++) {
		for (int y = 0; y < canvas->height;y++) {
			SET_BIT_COLOR(canvas,x,y,0);			
		}
	}

}


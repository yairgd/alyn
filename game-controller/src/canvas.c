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

#include "canvas.h"
#include "u8.h"

void canvas_init(struct canvas * canvas, const struct font * font, int width, int height)
{
	memset (canvas, 0, sizeof(*canvas));
	canvas->font = font;
	canvas->font_color = RGB(255,255,255);
	canvas->bg_color = RGB(0,0,0);


	canvas->width = width;
	canvas->height = height;
	canvas->buffer = malloc(4 * width * height);
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
	while (*str) {
		len = u8_to_unicode(str, &unicode );

		int char_value = UNICODE_TO_ISO8859_8(unicode);

		canvas_write_font(canvas,char_value,x+i*font_width(canvas->font,0) ,y);		
		i++;
		//printf("%x\n", char_value);
		str += len;		
	}


}


void canvas_fill_rect(const struct canvas * canvas, struct rect * rect,int color) {
	for (int x = rect->top_left_x; x < rect->buttom_right_x && x < canvas->width; x++) {
		for (int y = rect->top_left_y; y < rect->buttom_right_y && y < canvas->height; y++) {
			 SET_BIT_COLOR(canvas,x,y,color);	
		}
	}
}

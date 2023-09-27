/*
 * =====================================================================================
 *
 *       Filename:  banner.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2023 08:01:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "banner.h"
#include "u8.h"
#include <assert.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


int rect_width(struct rect * r) {
	return r->buttom_right_x - r->top_left_x;

}
int rect_height(struct rect * r){
	return r->buttom_right_y - r->top_left_y;	
}



/**
 * Created  09/25/2023
 * @brief   initze the canvas that is defined in the banner
 * @note  
 * @param   
 * @return  
 */
void banner_init(struct banner * banner, int width, int height) {
	canvas_free (&banner->canvas);
	canvas_init(&banner->canvas, width, height);
}




/**
 * Created  09/25/2023
 * @brief  initilize banner 
 * This function defined a recnatge that defined the part of the recnacge that is avalible for display
 * @note  
 * @param   tlx top left x of the banner to be avalive for display
 * @param   tly top left y
 * @param   brx buttom right x
 * @param   bry buttom right y
 * @return  
 */
void banner_set_rect(struct banner * banner, int tlx, int tly , int brx, int bry) {
	assert (brx > tlx);
	assert (bry > tly);

//	banner->r.buttom_right_x = brx;
//	banner->r.buttom_right_y = bry;
//	banner->r.top_left_x = tlx;
//	banner->r.top_left_y = tly;
	if (banner->rect_buffer) {
		free(banner->rect_buffer);
	}
	banner->rect_buffer = malloc ((brx - tlx) * (bry - tly) * 4);

}
/*
int banner_rect_width(struct banner * banner) {
	//return r->buttom_right_x - r->top_left_x;

}
int banner_rect_height(struct banner * banner){
	//return banner->r.buttom_right_y - banner->r.top_left_y;	
}
*/
/**
 * Created  09/25/2023
 * @brief  initilize banner  
 * @note  
 * @param    
 * @return  
 */
void banner_init_with_text(struct banner * banner, int x, int y,const char* fmt, ...){ 
	va_list args;
	va_start(args, fmt);

	vsnprintf(banner->text,sizeof(banner->text), fmt, args);

	int len = u8_strlen(banner->text);
	
	banner_init(banner, len * 6 /*font_width(banner->canvas.font,0)*/, 20 /*font_height(banner->canvas.font) */) ;

	
	canvas_print(&banner->canvas, x,y, banner->text);
}

void banner_set_text(struct banner * banner, int x, int y,const char* fmt, ...){ 
	va_list args;
	va_start(args, fmt);
	vsnprintf(banner->text,sizeof(banner->text), fmt, args);
}

/**
 * Created  09/26/2023
 * @brief   This routine is invoked upon each timeout expiration to update the banner state in accordance with its configuration
 * @note  
 * @param   
 * @return  
 */
void banner_manage() {
	//CONFIG_TIMER_HAS_64BIT_CYCLE_COUNTER
	// https://docs.zephyrproject.org/apidoc/latest/group__clock__apis.html
	//static uint64_t k_cycle_get_64 
	//	static uint32_t k_cycle_get_32 	( 	void  		) 	
	//	static int64_t k_uptime_delta 	( 	int64_t *  	reftime	) 	
}


void banner_set_blink_rate(struct banner * banner, int blink_rate) {
	banner->blink_rate = blink_rate;
}



/**
 * Created  09/26/2023
 * @brief   place data in the rectnage buffer and rerturn pointer of it.
 * The recangle buffer should be displyed on screen
 * @note  
 * @param   
 * @return  
 */
const char * banner_get_buffer(struct banner * banner, struct rect * r) {
	int w = r->buttom_right_x - r->top_left_x ;
	//int h = banner->r.top_left_y - banner->r.buttom_right_y;

	for (int y = r->top_left_y; y < r->buttom_right_y && y < banner->canvas.height; y ++) {
		for (int x = r->top_left_x; x < r->buttom_right_x && x < banner->canvas.width; x++) {
			int pixel = *(int*)&banner->canvas.buffer[4 * (x) + 4 * (y) * banner->canvas.width];
			*(int*)&banner->rect_buffer[4 * (x - r->top_left_x) + 4 * (y - r->top_left_y) * w ] = pixel;
		}
	}
	return banner->rect_buffer;
}


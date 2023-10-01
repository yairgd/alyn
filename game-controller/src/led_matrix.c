/*
 * =====================================================================================
 *
 *       Filename:  led_matrix.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/29/2023 01:25:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "led_matrix.h"
#include "canvas.h"
#include "banner.h"

#include <assert.h>
#include <stdlib.h>

void led_matrix_init(struct led_matrix * matrix,int width, int height) {
	canvas_init(&matrix->canvas, width, height);
	matrix->buffer = malloc (width * height * 4);
}
void led_matrix_free(struct led_matrix * matrix) {
	canvas_free(&matrix->canvas);
	free(matrix->buffer);
}

struct banner * led_matrix_get_banner(struct led_matrix * matrix, int id) {
	if (id>=0 && id<sizeof (matrix->banners)/sizeof(struct banner_location)) {
		return &matrix->banners[id].banner;
	}
	return 0;
}
 


struct frame * led_matrix_get_frame(struct led_matrix * matrix, int id) {
	return &matrix->frame;
}

char b[64*32*4];
void led_matrix_manage(struct led_matrix * matrix){
	
	// mange the banners and place its contetnt on canvs
	for (int i = 0; i < sizeof(matrix->banners) / sizeof(struct banner_location); i++) {
		if (banner_is_initialize(&matrix->banners[i].banner)) {
			banner_manage(&matrix->banners[i].banner); 
//			char * b = banner_get_canvas_buffer(&matrix->banners[i].banner);

			// get recnange from the canvas of the banner
			canvas_get_rect(&matrix->banners[i].banner.canvas, &(struct rect){0,0 , matrix->banners[i].r.width ,  matrix->banners[i].r.height   },b);

			//canvas_set_rect(&matrix->canvas, &(struct rect){matrix->banners[i].x,matrix->banners[i].y ,banner_width(&matrix->banners[i].banner), banner_height(&matrix->banners[i].banner)},b);
			
			// place the buffer of the banner in the canvas of the display	
			canvas_set_rect(&matrix->canvas, &(struct rect){matrix->banners[i].r.top_left_x,matrix->banners[i].r.top_left_y , matrix->banners[i].r.width ,  matrix->banners[i].r.height   },b);

		}
	}

	// manage other elements here. exmplae (round bar - TODO , it is element like banner - a differnce one)

	frame_manage(&matrix->frame);
}



/**
 * Created  09/29/2023
 * @brief   converts the canvsa buffer into data that fits to the RGB led martx
 * @note  
 * @param   
 * @return  
 */
void led_matrix_to_framebuffer(struct led_matrix * matrix){

}

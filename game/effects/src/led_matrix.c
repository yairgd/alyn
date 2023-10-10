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
#include "effect.h"

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


/**
 * Created  10/01/2023
 * @brief   return  pointer to banner
 * @note  
 * @param   led_matrix the RGB led matrix display handle
 * @param   id banner id
 * @param   r the rectange of the banner in the led_matrix
 * @return  
 */
struct effect_base * led_matrix_get_banner(struct led_matrix * matrix) {
	matrix->effects[matrix->idx] = banner_new();
	return matrix->effects[matrix->idx++];
}



struct effect_base * led_matrix_get_frame(struct led_matrix * matrix) {
	matrix->effects[matrix->idx] = frame_new();
	return matrix->effects[matrix->idx++];
}


//char b[64*32*4];
void led_matrix_manage(struct led_matrix * matrix){

	// mange the banners and place its contetnt on canvs
	for (int i = 0; i < 10; i++) {
		if (matrix->effects[i])
			effect_render(matrix->effects[i], &matrix->canvas, 0);

	}

}



/**
 * Created  09/29/2023
 * @brief   converts the canvas buffer into data that fits to the RGB led martx
 * @note  
 * @param   
 * @return  
 */
void led_matrix_to_framebuffer(struct led_matrix * matrix){

}

static struct led_matrix led_matrix = {0};

struct  led_matrix  *  get_led_matrix() {
	return  & led_matrix;
}

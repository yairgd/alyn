/*
 * =====================================================================================
 *
 *       Filename:  led_matrix.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/29/2023 01:25:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef LED_MATRIX_H
#define LED_MATRIX_H 

#include "banner.h"
#include "frame.h"
#include "canvas.h"

struct led_matrix {
	struct canvas canvas;
	char * buffer;
	struct banner_location {
		struct rect r; // rectangle inside led matrix		
		struct banner banner;
	} banners[3];

	struct frame frame;
};

void led_matrix_free(struct led_matrix * matrix);
void led_matrix_init(struct led_matrix * matrix,int width, int height);
void led_matrix_manage(struct led_matrix * matrix);
struct banner * led_matrix_get_banner(struct led_matrix * matrix, int id);
struct frame * led_matrix_get_frame(struct led_matrix * matrix, int id);


#endif

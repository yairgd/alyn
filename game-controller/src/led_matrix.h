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
struct led_matrix {
	struct canvas canvas;
	char * buffer;
	struct banner_location {
		int x,y;
		struct banner banner;
	} banners[3];
};

void led_matrix_free(struct led_matrix * matrix);
void led_matrix_init(struct led_matrix * matrix,int width, int height);
void led_matrix_manage(struct led_matrix * matrix);
struct banner * led_matrix_get_banner(struct led_matrix * matrix, int id);
#endif

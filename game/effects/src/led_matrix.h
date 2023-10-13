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


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "banner.h"
#include "frame.h"
#include "canvas.h"
#include "effect.h"

	struct led_matrix {
		struct canvas canvas;
		struct effect_base * effects[10];
		int idx;
		char * buffer;
	};

	void led_matrix_free(struct led_matrix * matrix);
	void led_matrix_init(struct led_matrix * matrix,int width, int height);
	void led_matrix_manage(struct led_matrix * matrix);
	struct effect_base * led_matrix_get_banner(struct led_matrix * matrix);
	struct effect_base * led_matrix_get_frame(struct led_matrix * matrix);
	struct  led_matrix  *  get_led_matrix();
	void led_matrix_begin_to_measure_time(struct led_matrix * matrix) ;
	int led_matrix_elapse_time(struct led_matrix * matrix, int64_t time);

#ifdef __cplusplus
}
#endif

#endif

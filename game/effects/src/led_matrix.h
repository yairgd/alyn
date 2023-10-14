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
	struct channel {
		struct canvas canvas;
		//struct effect_base * effects[10];
		//int idx;
		char * buffer;
		int opacity;			
	} ;
	struct led_matrix {
		struct channel channels[4];
		int width;
		int height;
	};

	void led_matrix_init(struct led_matrix *, int width, int height);
	void led_matrix_free(struct led_matrix *);
	void led_matrix_merge(struct led_matrix *,int c0, int c1, int c2);
	struct led_matrix * led_matrix_get();
	struct  channel  *  led_matrix_get_channel(struct led_matrix * matrix, int id);

#ifdef __cplusplus
}
#endif

#endif

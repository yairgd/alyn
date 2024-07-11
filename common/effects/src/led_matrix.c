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

static struct led_matrix led_matrix = {0};
static struct pixel  canvas_buffers[4][32*64] = {0};

void led_matrix_init(struct led_matrix * led_matrix, int width, int height) {

	led_matrix->width = width;
	led_matrix->height = height;

	for (int i = 0; i < sizeof (led_matrix->channels) / sizeof(struct channel) ; i++) {
		canvas_init(&led_matrix->channels[i].canvas, width, height, canvas_buffers[i] );
	}
}


void  led_matrix_free(struct led_matrix * led_matrix) {
	for (int i = 0; i < sizeof (led_matrix->channels) / sizeof(struct channel)  ; i++) {
		canvas_free(&led_matrix->channels[i].canvas);
	}
}


/**
 * Created  10/14/2023
 * @brief   sets the opacity level for channels 1,2,3
 * @note  
 * @param   
 * @return  
 */
void  led_matrix_set_opcaity(struct led_matrix * matrix, double o1, double o2, double o3) {
	matrix->channels[1].opacity = o1;
	matrix->channels[2].opacity = o2;
	matrix->channels[3].opacity = o3;
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


struct  channel  *  led_matrix_get_channel(struct led_matrix * matrix, int id) {
	return  &matrix->channels[id];
}


/**
 * Created  10/14/2023
 * @brief   merge channles 0,1,2 into channel 3
 * where non zero values from 0 are always at the top , chaneels 1 and 2 are merged by opacity level and then copied into channel 3
 * @note  
 * @param   
 * @return  
 */
void led_matrix_merge(struct led_matrix * matrix) {
	for (int row = 0; row < matrix->height; ++row)
	{
		for (int col = 0; col < matrix->width; ++col)
		{	

			struct pixel * c0  = GET_POINTER_TO_PIXEL(&(matrix->channels[0].canvas),col,row);
			struct pixel * c1  = GET_POINTER_TO_PIXEL(&(matrix->channels[1].canvas),col,row);
			struct pixel * c2  = GET_POINTER_TO_PIXEL(&(matrix->channels[2].canvas),col,row);

		//	int c0 = GET_BIT_COLOR(&(matrix->channels[0].canvas),col,row);
		//	int c1 = GET_BIT_COLOR(&(matrix->channels[1].canvas),col,row);
		//	int c2 = GET_BIT_COLOR(&(matrix->channels[2].canvas),col,row);

	//		int r2 = (c2>>16)&0x0000ff ; int r1 = (c1>>16) & 0x0000ff;
		//	int g2 = (c2>>8)&0x0000ff  ; int g1 = (c1>>8)  & 0x0000ff;
	//		int b2 = (c2>>0)&0x0000ff  ; int b1 = (c1>>0)  & 0x0000ff;


			uint8_t r2 = c2->r;uint8_t r1 = c1->r;
			uint8_t g2 = c2->g;uint8_t g1 = c1->g;
			uint8_t b2 = c2->b;uint8_t b1 = c1->b;

		

			//r1 = r1 ==0 ? r2 : r2==0 ? r1 : r2 * matrix->channels[2].opacity + r1 * matrix->channels[1].opacity;
			//g1 = g1 ==0 ? g2 : g2==0 ? g1 : g2 * matrix->channels[2].opacity + g1 * matrix->channels[1].opacity;
			//b1 = b1 ==0 ? b2 : r2==0 ? b1 : b2 * matrix->channels[2].opacity + b1 * matrix->channels[1].opacity;

			double o =  (matrix->channels[1].opacity +  matrix->channels[2].opacity + 0.0001);
			r1 =  (r2 * matrix->channels[2].opacity + r1 * matrix->channels[1].opacity)/o;
			g1 =  (g2 * matrix->channels[2].opacity + g1 * matrix->channels[1].opacity)/o;
			b1 =  (b2 * matrix->channels[2].opacity + b1 * matrix->channels[1].opacity)/o;

//			uint32_t cc = r1 << 16 | g1 <<8 | b1 << 0;

			struct pixel c = {r1,g1,b1,0};
			SET_BIT_COLOR(&(matrix->channels[3].canvas), col, row,  c0->r && c0->g && c0->b  ? c0 : &c) ;
		}
	}	
}


struct  led_matrix *  led_matrix_get() {
	return &led_matrix;
}

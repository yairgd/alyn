/*
 * =====================================================================================
 *
 *       Filename:  rect.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/12/2024 02:57:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "rect.h"

int rect_width(struct rect * r) {
	return r->width;

}
int rect_height(struct rect * r){
	return r->height;
}




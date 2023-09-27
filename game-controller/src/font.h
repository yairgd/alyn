/*
 * =====================================================================================
 *
 *       Filename:  font.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/19/2023 11:04:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef FONT_H
#define FONT_H 

#include "font_base.h"
extern const struct font * font_9x18();
extern const struct font * font_6x13();
const char * font_get_map(const struct font *, int idx);
int font_width(const struct font * font, int unicode);
int font_height(const struct font * font);

#endif

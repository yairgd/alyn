/*
 * =====================================================================================
 *
 *       Filename:  font.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/19/2023 11:04:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "font.h"


static int _font_get_index(const struct font * font, int font_id) {
	for (int i = 0; i < font->Chars ;  i++) {
		if (font->Index[i] == font_id)
			return i;
	}
	return -1;
}
int font_chars_per_row(const struct font * font) {
	return (font->Width / 8) + 1;
}
const char * font_get_map(const struct font * font, int font_id) {
	int byte_per_row = font_chars_per_row(font);
			      
	int idx = _font_get_index(font, font_id);
	return &font->Bitmap[idx * font->Height * byte_per_row];
}


/**
 * Created  09/26/2023
 * @brief   Get the height of the specified font. 
 * @note  
 * @param   font A pointer to the font structure.
 * @param   unicode  value of the charcater
 * @return  Retrieve the Unicode value to determine the specific width of the character
 */
int font_width(const struct font * font, int unicode) {
	return font->Width;
}

int font_height(const struct font * font)
{
	return font->Height;
	
}

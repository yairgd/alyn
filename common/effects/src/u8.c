/*
 * =====================================================================================
 *
 *       Filename:  u8.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/23/2023 12:31:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "u8.h"

/**
 * Created  09/23/2023
 * @brief   lengt hof unicode string
 * Returns the number of characters in an UTF-8 encoded string. 
 * (Does not check for encoding validity)
 * https://dev.to/rdentato/utf-8-strings-in-c-1-3-42a4
 * @note  
 * @param   
 * @return  
 */
int u8_strlen(const char *s)
{
  int len=0;
  while (*s) {
    if ((*s & 0xC0) != 0x80) len++ ;
    s++;
  }
  return len;
}

/**
 * Created  09/23/2023
 * @brief   extracts unicode value from UTF-8 encoding
 * refere for table here: https://www.fileformat.info/info/unicode/utf8.htm
 * @note  
 * @param   
 * @return  
 */
int u8_to_unicode(const char *s,int * unicode)
{
	int len = -1; // bad charcater
	*unicode = 0;
	
	if (0 < *(unsigned char*)s && *(unsigned char *)s < 0x7f) {
		// first and only byte of a sequence.
		*unicode = 0x7f & *s;
		len = 1;
	} else if (0xc2 < *(unsigned char *)s && *(unsigned char*)s < 0xdf) {
		// first byte of a two-byte sequence.
		*unicode = (*s & 0x1f) << 6 | (*(s+1) & 0x3f);
		len = 2;
	} else if (0xe0 < *(unsigned char*)s && *(unsigned char *)s < 0xef) {
		// first byte of a three-byte sequence
		*unicode = (*s & 0x0f) << 12 | (*(s+1) & 0x3f) << 6 | (*(s+1) & 0x3f) ;
		len = 3;
	} else if (0xf0 < *(unsigned char *)s && *(unsigned char *)s < 0xff) {
		// first byte of a four-byte sequence.
		*unicode = (*s & 0x07) << 18 | (*(s+1) & 0x3f) << 16 | (*(s+1) & 0x3f) << 6 |  (*(s+1) & 0x3f)   ;		
		len = 4;
	}
	return len;
}


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
#include "string.h"

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


// Helper function to convert a Unicode code point to a UTF-8 string
int unicode_to_utf8(int unicode, char *output) {
    if (unicode <= 0x7F) {
        output[0] = unicode;
        return 1;
    } else if (unicode <= 0x7FF) {
        output[0] = 0xC0 | (unicode >> 6);
        output[1] = 0x80 | (unicode & 0x3F);
        return 2;
    } else if (unicode <= 0xFFFF) {
        output[0] = 0xE0 | (unicode >> 12);
        output[1] = 0x80 | ((unicode >> 6) & 0x3F);
        output[2] = 0x80 | (unicode & 0x3F);
        return 3;
    } else if (unicode <= 0x10FFFF) {
        output[0] = 0xF0 | (unicode >> 18);
        output[1] = 0x80 | ((unicode >> 12) & 0x3F);
        output[2] = 0x80 | ((unicode >> 6) & 0x3F);
        output[3] = 0x80 | (unicode & 0x3F);
        return 4;
    }
    return -1; // Invalid Unicode code point
}

// Function to replace a character at a specific index in a UTF-8 string
void u8_replace(char *s, int idx, int unicode) {
    int char_idx = 0;
    int byte_idx = 0;
    int len = strlen(s);

    // Iterate through the string to find the character at the given index
    while (byte_idx < len) {
        int unicode_char;
        int char_len = u8_to_unicode(s + byte_idx, &unicode_char);

        if (char_len == -1) {
           // printf("Invalid UTF-8 encoding\n");
            return;
        }

        if (char_idx == idx) {
            char new_char[4];
            int new_char_len = unicode_to_utf8(unicode, new_char);

            if (new_char_len == -1) {
            //    printf("Invalid Unicode code point\n");
                return;
            }

            // Replace the character in the string
            int remaining_len = len - (byte_idx + char_len);
            memmove(s + byte_idx + new_char_len, s + byte_idx + char_len, remaining_len + 1);
            memcpy(s + byte_idx, new_char, new_char_len);

            return;
        }

        byte_idx += char_len;
        char_idx++;
    }

//    printf("Index out of range\n");
}


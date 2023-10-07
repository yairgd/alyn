/*
 * =====================================================================================
 *
 *       Filename:  u8.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/23/2023 12:32:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef U8_H
#define U8_H 

// take from here: https://chat.openai.com/c/3f992abf-9b42-46d9-919e-0d6829a4ba3d
// also look here: https://stackoverflow.com/questions/4607413/is-there-a-c-library-to-convert-unicode-code-points-to-utf-8
// Unicode to ISO 8859-8 Conversion Macro
#define UNICODE_TO_ISO8859_8(unicode) ( \
    ((unicode) >= 0x5D0 && (unicode) <= 0x5EA) ? ((unicode) - 0x5D0 + 0xE0) : ( \
    ((unicode) >= 0x5F0 && (unicode) <= 0x5F4) ? ((unicode) - 0x5F0 + 0xFA) : ( \
    ((unicode) == 0x20AC) ? 0xE1 : ( \
    ((unicode) == 0x2013) ? 0x97 : ( \
    ((unicode) == 0x2014) ? 0x96 : ( \
    ((unicode) == 0x2022) ? 0xF7 : ( \
    ((unicode) == 0x2026) ? 0xF6 : ( \
    ((unicode) == 0x203A) ? 0xFB : ( \
    ((unicode) == 0x20AA) ? 0xFD : ( \
    ((unicode) == 0x2122) ? 0xF8 : (unicode) \
    ))))))))))

// ISO 8859-8 to Unicode Conversion Macro
#define ISO8859_8_TO_UNICODE(c) ( \
    ((c) >= 0xA0 && (c) <= 0xFA) ? ((c) + 0x4F0) : ( \
    ((c) >= 0xE0 && (c) <= 0xFA) ? ((c) + 0x4E0) : ( \
    ((c) >= 0x80 && (c) <= 0x9F) ? ((c) + 0x300) : (c) \
)))

int u8_to_unicode(const char *s,int * unicode);
int u8_strlen(const char *s);
int u8_convert_to_iso(int unicode) ;	
#endif

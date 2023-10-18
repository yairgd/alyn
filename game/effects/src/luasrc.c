/*
 * =====================================================================================
 *
 *       Filename:  luasrc.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2023 08:57:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "luasrc.h"
#include <string.h>

struct luasrc * luasrc_by_name(char * name) 
{
	struct luasrc ** l = luasrc_get();
	while (*l) {
		if (strncmp ( (*l)->name , name,32) == 0)
			return *l;
		l++;
	}
	return 0;
}



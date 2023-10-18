/*
 * =====================================================================================
 *
 *       Filename:  luasrc.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2023 10:27:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef LUASRC_H
#define LUASRC_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
	struct luasrc  {
		char * name ;
		unsigned char * code;
		size_t size;
	};

	struct luasrc ** luasrc_get() ;
	struct luasrc * luasrc_by_name(char * name) ;

#ifdef __cplusplus
}
#endif
#endif

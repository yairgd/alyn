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

	const struct luasrc ** luasrc_get();
	const struct luasrc * luasrc_by_name(const char * name);
	const struct luasrc * luasrc_by_idx(int idx);
	size_t luasrc_size();
	extern const struct luasrc  * luasrc[];
#ifdef __cplusplus
}
#endif
#endif

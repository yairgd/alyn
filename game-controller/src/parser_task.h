/*
 * =====================================================================================
 *
 *       Filename:  parser_task.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2023 08:52:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef PARSER_TASK
#define PARSER_TASK 
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	void proccess_char(const  struct shell * sh, uint8_t *data, size_t len) ;

	
#ifdef __cplusplus
}
#endif

#endif

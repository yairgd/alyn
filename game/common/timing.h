/*
 * =====================================================================================
 *
 *       Filename:  timing.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/13/2023 08:37:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef TIMING_C
#define TIMING_C 
#include <stdint.h>
#include <stddef.h>

#ifdef _MSC_VER 
#include <time.h>
#elif __GNUC__
#include <unistd.h>
#endif


#ifndef ZEPHYR
#include <time.h>
#endif

void timing_sleep(size_t usec) ;
int timing_elapse(struct timespec  * start_time, double time);
void timing_begin_to_measure_time( struct timespec  * start_time );
#endif

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
#include <time.h>
#ifdef CONFIG_UART_NATIVE_POSIX
#include <sys/time.h>
#else
//#include <zephyr/posix/time.h>
//#include <zephyr/posix/sys/time.h>
#endif
#include <stdint.h>



int timing_elapse(struct timespec  * start_time, uint64_t time);
void timing_begin_to_measure_time( struct timespec  * start_time );
#endif

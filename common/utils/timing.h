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

#ifndef TIMING_H
#define TIMING_H 
#include <stdint.h>
#include <stddef.h>

void timing_sleep(size_t usec) ;
int timing_elapse(uint32_t /*struct timespec  **/ start_time, double time);
uint32_t timing_begin_to_measure_time( /*struct timespec  * start_time*/ );
#endif

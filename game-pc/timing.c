/*
 * =====================================================================================
 *
 *       Filename:  timing.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/13/2023 08:37:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <unistd.h>
//#include "timing.h"
#include <sys/time.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#define USEC_PER_SEC 1000000	


/**
 * Created  10/13/2023
 * @brief   returns true, if time elapse
 * @note  
 * @param   time(ms) to mesure
 * @return  
 */
uint32_t timing_elapse( uint32_t /*struct timespec  * */start_time, double time ) {
	struct timespec stop_time;

	int ret = clock_gettime(CLOCK_REALTIME, &stop_time);
	(void)ret;	

	return (uint32_t)stop_time.tv_sec * USEC_PER_SEC + (uint32_t)stop_time.tv_nsec/1000 - start_time > time * 1000 ;
}


uint32_t timing_begin_to_measure_time( ) {
	struct timespec stop_time;

	clock_gettime(CLOCK_REALTIME, &stop_time); // get system time 
	return (uint32_t)stop_time.tv_sec * USEC_PER_SEC + (uint32_t)stop_time.tv_nsec/1000 ;
}


/**
 * Created  10/19/2023
 * @brief   function as os hal for sleep function
 * @note  
 * @param   
 * @return  
 */
void timing_sleep(size_t usec) {
	usleep(usec );
}

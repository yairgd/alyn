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


//#include "timing.h"
#ifdef CONFIG_UART_NATIVE_POSIX
#include <sys/time.h>
#else
#include <zephyr/posix/time.h>
#include <zephyr/posix/sys/time.h>
#endif
#include <stddef.h>
#include <stdint.h>
#include <zephyr/kernel.h>


/**
 * Created  10/13/2023
 * @brief   returns true, if time elapse
 * @note  
 * @param   time(ms) to mesure
 * @return  
 */
int timing_elapse( struct timespec  * start_time, double time ) {

	struct timespec stop_time , *rts;
	rts = start_time;
#ifdef CONFIG_UART_NATIVE_POSIX
#  define NSEC_PER_SEC 1000000000
#else
	#define NSEC_PER_SEC 1000000000	
	int ret = clock_gettime(CLOCK_REALTIME, &stop_time);
	(void)ret;	
#endif
	double delta =
		((int64_t)stop_time.tv_sec * NSEC_PER_SEC -
		 (int64_t)rts->tv_sec * NSEC_PER_SEC) +
		((int64_t)stop_time.tv_nsec - (int64_t)rts->tv_nsec);
	return (delta/NSEC_PER_SEC) > time   ;
	
}


void timing_begin_to_measure_time( struct timespec  * start_time ) {
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	clock_gettime(CLOCK_REALTIME, start_time); // get system time 
#endif
}


/**
 * Created  10/19/2023
 * @brief   function as os hal for sleep function
 * @note  
 * @param   
 * @return  
 */
void timing_sleep(size_t usec) {
	k_sleep(K_USEC(usec));		
}

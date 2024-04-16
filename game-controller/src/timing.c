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
int timing_elapse( uint32_t /*struct timespec  * */start_time, double time ) {
	return k_uptime_get() - start_time > time;

}


uint32_t timing_begin_to_measure_time(  /*struct timespec   start_time*/ ) {
	return k_uptime_get();
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

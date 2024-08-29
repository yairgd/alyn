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
 *       Compiler:  MSVC
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <stdint.h>
#include <time.h>
#include <windows.h> // Windows-specific header for timing functions

#define USEC_PER_SEC 1000000	

/**
 * Created  10/13/2023
 * @brief   returns true, if time elapse
 * @note  
 * @param   time(ms) to measure
 * @return  
 */
uint32_t timing_elapse(uint32_t start_time, double time) {
    LARGE_INTEGER stop_time;
    LARGE_INTEGER frequency;
    
    QueryPerformanceFrequency(&frequency); // Get the frequency of the performance counter
    QueryPerformanceCounter(&stop_time);   // Get the current time
    
    double elapsed_usec = (double)(stop_time.QuadPart) / (frequency.QuadPart / USEC_PER_SEC);
    return (uint32_t)(elapsed_usec - start_time) > time * 1000;
}

uint32_t timing_begin_to_measure_time() {
    LARGE_INTEGER start_time;
    LARGE_INTEGER frequency;
    
    QueryPerformanceFrequency(&frequency); // Get the frequency of the performance counter
    QueryPerformanceCounter(&start_time);  // Get the current time

    return (uint32_t)(start_time.QuadPart / (frequency.QuadPart / USEC_PER_SEC));
}

/**
 * Created  10/19/2023
 * @brief   function as os hal for sleep function
 * @note  
 * @param   
 * @return  
 */
void timing_sleep(size_t usec) {
    Sleep((DWORD)(usec / 1000)); // Sleep expects milliseconds, so convert from microseconds
}


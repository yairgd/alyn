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


#include "timing.h"

#ifdef _MSC_VER
#include <Windows.h>
#define CLOCK_REALTIME 0
//https://stackoverflow.com/questions/5404277/porting-clock-gettime-to-windows

static LARGE_INTEGER
getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

static int
clock_gettime(int X, struct timeval* tv)
{
    LARGE_INTEGER           t;
    FILETIME            f;
    double                  microseconds;
    static LARGE_INTEGER    offset;
    static double           frequencyToMicroseconds;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        }
        else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}

#endif

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

	#define NSEC_PER_SEC 1000000000	
	int ret = clock_gettime(CLOCK_REALTIME, &stop_time);
	(void)ret;	
	//https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/timespec-get-timespec32-get-timespec64-get1?view=msvc-170
	double delta =
		((int64_t)stop_time.tv_sec * NSEC_PER_SEC -
		 (int64_t)rts->tv_sec * NSEC_PER_SEC) +
		((int64_t)stop_time.tv_nsec - (int64_t)rts->tv_nsec);
	return (delta/NSEC_PER_SEC)  > time   ;
	
}


void timing_begin_to_measure_time( struct timespec  * start_time ) {
	clock_gettime(CLOCK_REALTIME, start_time); // get system time 
}



/**
 * Created  10/19/2023
 * @brief   function as os hal for sleep function
 * @note  
 * @param   
 * @return  
 */
void timing_sleep(size_t usec) {
	Sleep(usec);
}

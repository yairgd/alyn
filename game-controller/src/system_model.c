/*
 * =====================================================================================
 *
 *       Filename:  system_model.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/05/2023 10:01:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "system_model.h"

struct system_model model;

void set_active_game(int id) {
	model.active_game = id;
}

void set_rgb(int id, int r, int g, int b) {
	model.stations[id].r = r;
	model.stations[id].g = g;
	model.stations[id].b = b;
}

int get_r(int id) {
	return model.stations[id].r ;
}
int get_g(int id) {
	return model.stations[id].g ;
}
int get_b(int id) {
	return model.stations[id].b ;
}


int get_input(int id) {
	return model.stations[id].button_state;
}

int get_active_game() {
	return model.active_game;
}
#if 0
void set_button_press(int id) {
	model.stations[id].button_state = 1;
	model.stations[id].time_since_press = 0 ; // get system time in ms 
}

#endif

void start_blink(int id, int freq, double blink_time) {
	model.stations[id].blink.freq = freq; 	
	model.stations[id].blink.blink = 1; 	
	model.stations[id].blink.state = 0;
	model.stations[id].blink.max_blink_time = blink_time;
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	clock_gettime(CLOCK_REALTIME, &model.stations[id].blink.ts); // get system time 
	clock_gettime(CLOCK_REALTIME, &model.stations[id].blink.bts); // get system time	
#endif

} 

void stop_blink(int id) {
	model.stations[id].blink.blink = 0; 	

}
void toggle_led(int id) {
	// update last toggle time
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	clock_gettime(CLOCK_REALTIME, &model.stations[id].blink.bts); 
#endif
	model.stations[id].blink.state= model.stations[id].blink.state == 0 ? 1 : 0;	
}


double time_elapse_since_start_blink(int id) {
	struct timespec nts , *rts;
	rts = &model.stations[id].blink.ts;
#ifdef CONFIG_UART_NATIVE_POSIX
#  define NSEC_PER_SEC 1000000000
#else
	int ret = clock_gettime(CLOCK_REALTIME, &nts);
	(void)ret;	
#endif
	double delta =
		((int64_t)nts.tv_sec * NSEC_PER_SEC -
		 (int64_t)rts->tv_sec * NSEC_PER_SEC) +
		((int64_t)nts.tv_nsec - (int64_t)rts->tv_nsec);
	return delta/NSEC_PER_SEC;
} 

int is_blink(int id) {
	if (time_elapse_since_start_blink(id) > model.stations[id].blink.max_blink_time) { 
		model.stations[id].blink.blink = 0;
		model.stations[id].blink.stop_reson = 2; // time out
	}
	return model.stations[id].blink.blink;
}

void set_timeout_reson(int id, int v)
{
	model.stations[id].blink.stop_reson = v;
}
int time_elapse_since_last_toggle(int id) {
	struct timespec nts , *rts;
	rts = &model.stations[id].blink.bts;
#ifdef CONFIG_UART_NATIVE_POSIX
#else
	int ret = clock_gettime(CLOCK_REALTIME, &nts);
	(void)ret;
#endif
	double delta =
		((int64_t)nts.tv_sec * NSEC_PER_SEC -
		 (int64_t)rts->tv_sec * NSEC_PER_SEC) +
		((int64_t)nts.tv_nsec - (int64_t)rts->tv_nsec);
	return delta/NSEC_PER_SEC > 1.0 / model.stations[id].blink.freq;
} 


int is_blink_on(int id) {
	return model.stations[id].blink.state;
}


void set_button_release(int id) {
	model.stations[id].button_state = 0;
}


void set_active_node(int id) {
	model.active_node = id;
}
int get_active_node() {
	return model.active_node ;
}


void set_free_run(int v) {
	model.free_run = v;
};
int get_free_run() {
	return model.free_run;
}


void set_free_run_delay(int v) {
	model.free_run_delay_us = v;
}

int get_free_run_delay() {
	return model.free_run_delay_us;
}

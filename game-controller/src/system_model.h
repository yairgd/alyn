/*
 * =====================================================================================
 *
 *       Filename:  system_model.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/05/2023 09:56:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef SYSTEM_MODEL
#define SYSTEM_MODEL 
#ifdef CONFIG_UART_NATIVE_POSIX
#include <sys/time.h>
#else
#include <zephyr/posix/time.h>
#include <zephyr/posix/sys/time.h>
#endif
#include <stdint.h>


struct system_model {
	struct  {
		unsigned int r 				: 8;
		unsigned int g 				: 8;
		unsigned int b 				: 8;
		unsigned int active 			: 1;  // 0 - off, 1 -on
		unsigned int button_state 		: 1;  //0 press 
		struct blink_info {
			struct timespec ts;		// time size start blink command
			struct 	timespec bts;		// time sice change of blick state			
			unsigned short state		: 1;  // 1 - the blink on state , 0 - the blonk off state
			unsigned short blink		: 1;	
			unsigned short freq  		: 8;  // blinks per seconds
			unsigned short stop_reson	: 2; // 0 - not deined , 1 time out , 2 button press
			double max_blink_time;  		// the system counteer at the press tim
		} blink;
	} stations[8];
	unsigned short active_node 			: 3;
	unsigned short active_game 			: 8;
	unsigned short free_run 			: 1;
	
	int free_run_delay_us;
	

};

void set_active_game(int id);
void set_rgb(int id, int r, int g, int b);
int get_input(int id);
int get_active_game();
void set_button_press(int id);
void set_button_release(int id);
void set_active_node(int id);
int get_active_node();
int get_r(int id);
int get_g(int id);
int get_b(int id);
void set_free_run(int v);
int get_free_run();

void start_blink(int id, int freq, double blink_time);
double time_ellepse_since_start_blink(int id);
void set_free_run_delay(int v);
int get_free_run_delay();
void stop_blink(int id);
int is_blink(int id);
double time_elapse_since_start_blink(int id);
int time_elapse_since_last_toggle(int id);
void toggle_led(int id);
int is_blink_on(int id);
void set_timeout_reson(int id, int v);

#endif


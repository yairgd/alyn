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
#include "timing.h"
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

	struct system_model {
		struct  {
			unsigned char r 		;	
			unsigned char g 		;		
			unsigned char b 		;
			unsigned int connected 			: 1;  // 0 - station not connected, 1 station is conncted
			unsigned int button_state 		: 1;  //0 press 
			struct blink_info {
			//	struct timespec ts;		// time sice change of blick state
			//	struct 	timespec start_blink_ts;// time sice start blink		
				char ts[100];
				char start_blink_ts[100];
				unsigned short state		: 1;  // the blinking state: 1 - led on , 0 - led off
				unsigned short active		: 1;  // 1 - blink is active, 0 blink off	
				unsigned short freq  		: 8;  // blinks per seconds
				unsigned short stop_reson	: 2; // 0 - not deined , 1 time out , 2 button press
				double max_blink_time;  		// the system counteer at the press tim
			} blink;
		} stations[8];
		unsigned short active_node 			: 3;
		unsigned short active_game 			: 8;
		unsigned short free_run 			: 1;
		unsigned short key_pad				: 5;    // bit 0: middle key, 1: left key, 2: right key, 3: up key, 4: down key
		unsigned short long_key_pad			: 5;    // bit 0: middle key, 1: left key, 2: right key, 3: up key, 4: down key
		int free_run_delay_us;		
		


	};
	int get_rgb(int id);

	void set_active_game(int id);
	void set_rgb(int id, int r, int g, int b);
	int get_input(int id);
	int get_active_game();
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
	//double time_elapse_since_start_blink(int id);
	//int time_elapse_since_last_toggle(int id);
	void toggle_led(int id);
	int is_blink_on(int id);
	void set_timeout_reson(int id, int v);
	void manage_blink(int id) ;
	void set_connected(int id, int c);
	int is_connected(int id) ;

	void set_button_state(int id, int b);
	int get_button_state(int id);
	int get_stop_reason(int id) ;

	void set_key(int key, int v);
	int get_keys();
	
	void set_long_key(int key, int v);
	int get_long_keys();
	

#ifdef __cplusplus
}
#endif

#endif


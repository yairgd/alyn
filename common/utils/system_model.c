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
#include "hal/timing.h"
//#define CHECK_ID(id) ((1<= (id) && (id)<=8)  ? (id) - 1 : 0 )
#define CHECK_ID(id) (id)

static struct system_model model;

void set_active_game(int id) {
	model.active_game = id;
}

void set_rgb(int id, int r, int g, int b) {
	id = CHECK_ID(id);
	
	model.stations[id].r = r;
	model.stations[id].g = g;
	model.stations[id].b = b;
}
int get_rgb(int id) {
	id = CHECK_ID(id);
	
	return model.stations[id].r << 16 |   model.stations[id].g<<8 | model.stations[id].b;
}
int get_r(int id) {
	id = CHECK_ID(id);

	return model.stations[id].r ;
}
int get_g(int id) {
	id = CHECK_ID(id);

	return model.stations[id].g ;
}
int get_b(int id) {
	id = CHECK_ID(id);

	return model.stations[id].b ;
}


int get_input(int id) {
	id = CHECK_ID(id);

	return model.stations[id].button_state;
}

int get_active_game() {
	return model.active_game;
}

void start_blink(int id, int freq, double blink_time) {
	id = CHECK_ID(id);

	model.stations[id].blink.freq = freq; 	
	model.stations[id].blink.active = 1; 	
	model.stations[id].blink.state = 0;
	model.stations[id].blink.max_blink_time = blink_time;
	model.stations[id].blink.stop_reson = 0;
	model.stations[id].blink.start_blink_ts = timing_begin_to_measure_time();
} 

void stop_blink(int id) {
	model.stations[id].blink.active = 0; 	

}





int is_blink(int id) {
	if (model.stations[CHECK_ID(id)].blink.active && timing_elapse(model.stations[CHECK_ID(id)].blink.start_blink_ts, model.stations[CHECK_ID(id)].blink.max_blink_time)) { 
		// blinking timeout
		model.stations[CHECK_ID(id)].blink.active = 0;
		model.stations[CHECK_ID(id)].blink.stop_reson = 2; // time out
	}

	if (model.stations[CHECK_ID(id)].blink.active && get_button_state(id)   ) { 
		if (is_connected(id)) {
			// stop blinking becuse button is pressed
			model.stations[CHECK_ID(id)].blink.active = 0;
			model.stations[CHECK_ID(id)].blink.stop_reson = 1; // button press
		}
		set_button_state(id,0);
	}

	
	return model.stations[CHECK_ID(id)].blink.active;
}


void set_timeout_reson(int id, int v)
{
	id = CHECK_ID(id);

	model.stations[id].blink.stop_reson = v;
}

void toggle_led(int id) {
	id = CHECK_ID(id);

	model.stations[id].blink.ts= timing_begin_to_measure_time();
	model.stations[id].blink.state= model.stations[id].blink.state == 0 ? 1 : 0;	
}




/**
 * Created  10/21/2023
 * @brief   returns the blink state: 0 off , 1 on - relevance only if blink is active
 * @note  
 * @param   
 * @return  
 */
int is_blink_on(int id) {
	id = CHECK_ID(id);

	return model.stations[id].blink.state;
}


void set_button_release(int id) {
	id = CHECK_ID(id);

	model.stations[id].button_state = 0;
}


void set_active_node(int id) {
	id = CHECK_ID(id);

	model.active_node = id;
}
int get_active_node() {

	return model.active_node ;
}


void set_free_run(int v) {

	model.free_run = v;
};



/**
 * Created  10/19/2023
 * @brief   enable free run mode (were each station is turned of/off  in it time)
 * @note  
 * @param   
 * @return  
 */
int get_free_run() {
	return model.free_run;
}


/**
 * Created  10/19/2023
 * @brief   sets the delay between led switch at free run mode
 * @note  
 * @param   
 * @return  
 */
void set_free_run_delay(int v) {
	model.free_run_delay_us = v;
}


int get_free_run_delay() {
	return model.free_run_delay_us;
}



/**
 * Created  10/21/2023
 * @brief   turn on/off led in blink state
 * @note  
 * @param   
 * @return  
 */
void manage_blink(int id) {
	double blink_timeout = 1.0 / model.stations[CHECK_ID(id)].blink.freq;
	if ( is_blink(id) &&timing_elapse(model.stations[CHECK_ID(id)].blink.ts, blink_timeout * 1000) ) {
		toggle_led(id);			
	}
}




/**
 * Created  10/21/2023
 * @brief   sets the station connection: 0 connected ,1 not connected
 * @note  
 * @param   
 * @return  
 */
void set_connected(int id, int c) {
	id = CHECK_ID(id);

	model.stations[id].connected = c;
	
}


int is_connected(int id) {
	id = CHECK_ID(id);
	return 	model.stations[id].connected ;
}


/**
 * Created  10/21/2023
 * @brief   sets station button state: 0, not press, 1- > button pressed
 * @note  
 * @param   
 * @return  
 */
void set_button_state(int id, int b) {
	model.stations[CHECK_ID(id)].button_state = b;
}

int get_button_state(int id) {
	id = CHECK_ID(id);
	return model.stations[id].button_state;
}


int get_stop_reason(int id) {
	id = CHECK_ID(id);
	return model.stations[id].blink.stop_reson;
}



/**
 * Created  10/22/2023
 * @brief   set key state :1 press, 0 - not press
 * @note  
 * @param   
 * @return  
 */
void set_key(int key, int v) {
	model.key_pad = v ? model.key_pad | (1<<key) : model.key_pad & ~(1<<key);
}

int get_keys() {
	return model.key_pad ;
}


void set_long_key(int key, int v) {
	model.long_key_pad = v ? model.key_pad | (1<<key) : model.key_pad & ~(1<<key);
}
int get_long_keys() {
	return model.long_key_pad ;

}


void timer_set(struct timer * t) {
	model.timer = *t;
	model.timer.ms_counter = timing_begin_to_measure_time();		
}

uint32_t timer_get() {
	return model.timer.seconds;
}

static void timer_update() {
	if (model.timer.dir == 1){
		if (model.timer.seconds < (99 * 60) +59) {
			model.timer.seconds++;
		}
	} else {
		if (model.timer.seconds > 0) {
			model.timer.seconds--;	
		}
	}
	model.timer.ms_counter = timing_begin_to_measure_time();	
}


void timer_manage() {	
	if ( timing_elapse(model.timer.ms_counter, 1000))  {
		timer_update();
	}

}

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

void set_button_press(int id) {
	model.stations[id].button_state = 1;
	model.stations[id].time_since_press = 0 ; // get system time in ms 
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

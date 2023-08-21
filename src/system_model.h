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

struct system_model {
	struct  {
		unsigned int r 			: 8;
		unsigned int g 			: 8;
		unsigned int b 			: 8;
		unsigned int active 		: 1;  // 0 - off, 1 -on
		unsigned int button_state 	: 1;  //0 press 
		unsigned long time_since_press;
	} stations[8];
	unsigned short active_node 		: 3;
	unsigned short active_game 		: 8;
	unsigned short free_run 		: 1;
	
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


void set_free_run_delay(int v);
int get_free_run_delay();

#endif


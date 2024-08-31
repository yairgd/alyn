/*
 * =====================================================================================
 *
 *       Filename:  game.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/24/2023 04:01:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef game_h
#define game_h 
#include <stddef.h>
#include <stdio.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/sys/util.h>
#ifdef __cplusplus
extern "C" {
#endif

struct game {
	void (*func)(void *data);
	void *data;
	char name[32];
	sys_dnode_t node; // Doubly linked list node structure
};
void game_show_list();
void game_init(void); 
void game_run(struct game *);
struct game * game_get_by_index(int i);
void game_stop();
void game_start(struct game *g);
void push_lua_code_to_memory(unsigned char *code, int len, int rst);
#ifdef __cplusplus
}
#endif

#endif

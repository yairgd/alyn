/*
 * =====================================================================================
 *
 *       Filename:  game.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/24/2023 04:01:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <string.h>

#include "game.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <zephyr/kernel.h>
#include "luasrc.h"
#include "system_model.h"

#define MY_STACK_SIZE 16384
#define MY_PRIORITY 5
static sys_dnode_t head;
static lua_State *L = 0;


K_THREAD_STACK_DEFINE(game_thread_area, MY_STACK_SIZE);
struct k_thread game_thread;
k_tid_t game_thread_tid = 0;


static unsigned char memory_code_buffer[8192];
struct  luasrc   lua_memory_game = {
	.code = memory_code_buffer,
};



static void game_thread_entry(void *g, void *a, void *b) {
	struct game * game = g;
	if (game->func) {
		game->func(game->data);
	}
}

void push_lua_code_to_memory(unsigned char *code, int len, int rst){
	static int idx = 0;
	int l = len > sizeof(memory_code_buffer) ? sizeof(memory_code_buffer)  : len;

	if (rst) {
		idx = 0;
		lua_memory_game.name = "lua code in memory";
		//strncpy (lua_memory_game.name, "lua code in memory", sizeof(lua_memory_game.name));
	}
	memcpy ((void*)&lua_memory_game.code[idx], code, l);
	idx += l;
	lua_memory_game.size = idx;
	if (idx > sizeof(memory_code_buffer))
		idx =0;

};



static void game_default_c(void *data) {
	int cnt = 0;
	while (cnt < 10) {
		k_sleep(K_MSEC(1000));		
		printf("run game %d\n",cnt++);
	}
	return;
}



static void game_lua_generic(void *data) {
	struct luasrc * game = data;

	printf("start runnunig lua:  %s\n", game->name);
	if (L)
		lua_close(L); 
	L = luaL_newstate();
	luaL_openlibs(L);
#if 0
	if (luaL_dostring(L, lua)) {
		fprintf(stderr, "%s\n", lua_tostring(L,1));
	}
#endif
	int error =
		luaL_loadbuffer( L, game->code, game->size, "main" )
		|| lua_pcall( L, 0, 0, 0 );

	if (error) {
		fprintf(stderr, "%s\n", lua_tostring(L,1));

	}
	lua_close(L);   /* Cya, Lua */
	L = 0;
	printf("finish runnunig lua:  %s\n", game->name);

}



static struct game games[16] = {
	{.name = "game in meory",.func = game_lua_generic, .data = &lua_memory_game  },
	{.name = "default c game",.func = game_default_c},
	{0},
};

void game_init(void) {
	struct game * g = games;
	// Initialize the doubly linked list
	sys_dlist_init(&head);

	/* this add compiled lua src's to linked list */
	for (int i = 0; i < luasrc_size() && i < 16; i++) {
		struct luasrc * src = luasrc_by_idx(i);
		strncpy (games[i+2].name , src->name, 32);
		games[i+2].func = game_lua_generic;
		games[i+2].data = src;
	}

	// Populate the data in games	
	while (g->func)  {
		sys_dlist_append(&head, &g->node);
		g++;
	}


}

void game_show_list() {
	sys_dnode_t *cur_node;
	int i = 0;


	printf("\nList of games:\n");
	SYS_DLIST_FOR_EACH_NODE(&head, cur_node) {
		struct game *game = CONTAINER_OF(cur_node, struct game, node);
		printf("%2d -> %s \n", ++i, game->name);
	}
}

struct game * game_get_by_index(int i) {
	int j = 0;
	struct game *game = 0;
	sys_dnode_t *cur_node;

	SYS_DLIST_FOR_EACH_NODE(&head, cur_node) {
		game = CONTAINER_OF(cur_node, struct game, node);
		if (i == j) {
			break;
		}
		j++;
	}
	return game;
}


void game_start(struct game *g) {
	if (g != NULL) {
		game_stop();
		game_thread_tid = k_thread_create(&game_thread, game_thread_area,
				K_THREAD_STACK_SIZEOF(game_thread_area),
				game_thread_entry,
				g, NULL, NULL,
				MY_PRIORITY, 0, K_NO_WAIT);
		k_thread_start(game_thread_tid);
	}

}
void game_stop() {
	if (game_thread_tid) {
		k_thread_abort(game_thread_tid);	
		game_thread_tid = NULL;
		set_active_game(255);
	}	
}	

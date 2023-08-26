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

#include "game.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <zephyr/kernel.h>
#include "luasrc.h"

#define MY_STACK_SIZE 16384
#define MY_PRIORITY 5
static sys_dnode_t head;
static lua_State *L;


K_THREAD_STACK_DEFINE(game_thread_area, MY_STACK_SIZE);
struct k_thread game_thread;
k_tid_t game_thread_tid = 0;

#if 0
static  char lua1[] = " \
function wait(seconds)\
    local start = os.time() \
    while os.time() - start < seconds do \
    end \
end \
for i = 10,1,-1 \
do \
   game.enable(i % 8 + 1)\
   game.delay(1000000); \
end \
";
#endif

static void game_thread_entry(void *g, void *a, void *b) {
	struct game * game = g;
	if (game->func) {
		game->func(game->data);
	}
}





static void game_default_c(void *data) {
	int cnt = 0;
	while (cnt < 10) {
		k_sleep(K_MSEC(1000));		
		printf("run game %d\n",cnt++);
	}
	return;
}



static void game_lua_generic(void *data) {
	const char * lua =  data;

	L = luaL_newstate();
	luaL_openlibs(L);
#if 0
	if (luaL_dostring(L, lua)) {
		fprintf(stderr, "%s\n", lua_tostring(L,1));
	}
#endif
	int error =
       luaL_loadbuffer( L, g_lua_game1, sizeof( g_lua_game1 ), "main" )
       || lua_pcall( L, 0, 0, 0 );

	if (error) {
		fprintf(stderr, "%s\n", lua_tostring(L,1));

	}
	lua_close(L);   /* Cya, Lua */
}



static struct game games[] = {
	{.name = "default c game",.func = game_default_c},
	{.name = "default lua game", .func = game_lua_generic, .data = g_lua_game1  },
};





void game_init(void) {

	// Initialize the doubly linked list
	sys_dlist_init(&head);

	// Populate the data in games
	for (int i = 0; i < sizeof(games)/sizeof(struct game); ++i) {
		sys_dlist_append(&head, &games[i].node);

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
	}
	return game;
}


void game_start(struct game *g) {

	if (g) {
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
		set_active_game(255);
	}	
}	

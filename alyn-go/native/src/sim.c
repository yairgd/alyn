#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "led_matrix.h"
#include "luasrc.h"
#include "system_model.h"
#include "sim.h"

static pthread_t game_th;
static int game_alive;
static lua_State *L;
static pthread_mutex_t game_mu = PTHREAD_MUTEX_INITIALIZER;

static struct {
	const char *code;
	size_t size;
	char *own;     /* malloc'd source for :load, freed on next start */
} job;

static void *tick_main(void *arg)
{
	(void)arg;
	for (;;) {
		timer_manage();
		for (int i = 0; i < SIM_STATIONS; i++)
			manage_blink(i);
		usleep(25000);
	}
	return NULL;
}

static void sim_start_ticks(void)
{
	pthread_t t;
	pthread_create(&t, NULL, tick_main, NULL);
	pthread_detach(t);
}

static void *game_main(void *arg)
{
	(void)arg;
	if (L)
		lua_close(L);
	L = luaL_newstate();
	luaL_openlibs(L);
	int err = luaL_loadbuffer(L, job.code, job.size, "main") || lua_pcall(L, 0, 0, 0);
	if (err) {
		const char *m = lua_tostring(L, -1);
		console_write(m, (int)strlen(m));
		console_write("\n", 1);
	}
	game_alive = 0;
	return NULL;
}

static void stop_locked(void)
{
	if (game_alive) {
		pthread_cancel(game_th);
		pthread_join(game_th, NULL);
		game_alive = 0;
	}
	free(job.own);
	job.own = NULL;
}

static int start_locked(void)
{
	game_alive = 1;
	if (pthread_create(&game_th, NULL, game_main, NULL)) {
		game_alive = 0;
		return -1;
	}
	return 0;
}

int sim_game_start(int idx)
{
	const struct luasrc *g = luasrc_by_idx(idx);
	if (!g)
		return -1;
	pthread_mutex_lock(&game_mu);
	stop_locked();
	job.code = g->code;
	job.size = g->size;
	int r = start_locked();
	pthread_mutex_unlock(&game_mu);
	return r;
}

int sim_game_start_source(const char *src, size_t len)
{
	pthread_mutex_lock(&game_mu);
	stop_locked();
	job.own = malloc(len + 1);
	if (!job.own) {
		pthread_mutex_unlock(&game_mu);
		return -1;
	}
	memcpy(job.own, src, len);
	job.own[len] = 0;
	job.code = job.own;
	job.size = len;
	int r = start_locked();
	pthread_mutex_unlock(&game_mu);
	return r;
}

void sim_game_stop(void)
{
	pthread_mutex_lock(&game_mu);
	stop_locked();
	pthread_mutex_unlock(&game_mu);
}

int sim_game_running(void)
{
	return game_alive;
}

int sim_game_count(void)
{
	return (int)luasrc_size();
}

const char *sim_game_name(int idx)
{
	const struct luasrc *g = luasrc_by_idx(idx);
	return g ? g->name : NULL;
}

void sim_init(void)
{
	led_matrix_init(led_matrix_get(), SIM_W, SIM_H_);
	for (int i = 0; i < SIM_STATIONS; i++)
		set_connected(i, 1);   /* the Qt LED widgets start connected */
	console_out_fd();
	sim_start_ticks();
}

void sim_key(int id, int is_long)
{
	if (id < 0 || id > 4)
		return;
	if (is_long)
		set_long_key(id, 1);
	else
		set_key(id, 1);
}

void sim_button(int station)
{
	if (station >= 0 && station < SIM_STATIONS)
		set_button_state(station, 1);
}

void sim_connect(int station, int on)
{
	if (station >= 0 && station < SIM_STATIONS)
		set_connected(station, on);
}

void sim_frame(unsigned char *out)
{
	struct led_matrix *m = led_matrix_get();
	struct channel *ch;

	led_matrix_merge(m);
	ch = led_matrix_get_channel(m, 3);
	for (int y = 0; y < SIM_H_; y++)
		for (int x = 0; x < SIM_W; x++) {
			struct pixel *p = GET_POINTER_TO_PIXEL(&ch->canvas, x, y);
			unsigned char *o = out + (y * SIM_W + x) * 3;
			o[0] = p->r;
			o[1] = p->g;
			o[2] = p->b;
		}
	out += SIM_W * SIM_H_ * 3;
	for (int i = 0; i < SIM_STATIONS; i++) {
		int rgb = get_rgb(i);
		out[i * 4 + 0] = (rgb >> 16) & 0xff;
		out[i * 4 + 1] = (rgb >> 8) & 0xff;
		out[i * 4 + 2] = rgb & 0xff;
		out[i * 4 + 3] = (is_connected(i) ? ST_CONNECTED : 0) |
				 (is_blink(i) ? ST_BLINK : 0) |
				 (is_blink_on(i) ? ST_BLINK_ON : 0);
	}
}

/* The USE_GUI_CONSOLE hooks of the Lua REPL (lua.c:458, lauxlib.h:264), same contract as
 * the Qt terminaltextedit.cpp, but feeding the Go widget through a pipe and a condition:
 *   print / errors  -> write_line / write_format_line -> pipe
 *   prompt          -> readline(prompt)               -> pipe, then blocks for a line
 * No Go callbacks: the game thread also prints, and it can be pthread_cancel'ed. */
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sim.h"

extern int lua_main(int argc, char **argv);

static int pipefd[2] = { -1, -1 };
static pthread_mutex_t out_mu = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t in_mu = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t in_cv = PTHREAD_COND_INITIALIZER;
static char *pending;
static int at_eof;

int console_out_fd(void)
{
	if (pipefd[0] < 0 && pipe(pipefd))
		return -1;
	return pipefd[0];
}

static void put_chunk(int kind, const char *s, int len)
{
	int old;

	if (console_out_fd() < 0)
		return;
	/* never be cancelled holding the lock or half a chunk */
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
	pthread_mutex_lock(&out_mu);
	while (len > 0) {
		unsigned char buf[5 + 4000];
		int n = len > 4000 ? 4000 : len;
		buf[0] = (unsigned char)kind;
		buf[1] = n;
		buf[2] = n >> 8;
		buf[3] = n >> 16;
		buf[4] = n >> 24;
		memcpy(buf + 5, s, n);
		if (write(pipefd[1], buf, 5 + n) < 0)
			break;
		s += n;
		len -= n;
	}
	pthread_mutex_unlock(&out_mu);
	pthread_setcancelstate(old, NULL);
}

void console_write(const char *s, int len)
{
	put_chunk(CON_OUT, s, len);
}

void write_line(const char *s, int l)
{
	console_write(s, l);
}

void write_format_line(char *s, const char *p)
{
	char str[512];

	snprintf(str, sizeof str, s, p);
	console_write(str, (int)strlen(str));
	console_write("\n", 1);
}

void add_history(const char *line)
{
	(void)line;
}

char *readline(const char *prompt)
{
	char *line;

	put_chunk(CON_PROMPT, prompt, (int)strlen(prompt));
	pthread_mutex_lock(&in_mu);
	while (!pending && !at_eof)
		pthread_cond_wait(&in_cv, &in_mu);
	line = pending;   /* lua frees it */
	pending = NULL;
	pthread_mutex_unlock(&in_mu);
	return line;
}

void console_send_line(const char *line)
{
	pthread_mutex_lock(&in_mu);
	free(pending);
	pending = strdup(line);
	pthread_cond_signal(&in_cv);
	pthread_mutex_unlock(&in_mu);
}

void console_eof(void)
{
	pthread_mutex_lock(&in_mu);
	at_eof = 1;
	pthread_cond_broadcast(&in_cv);
	pthread_mutex_unlock(&in_mu);
}

int console_run(void)
{
	char *argv[] = { "alyn", NULL };

	pthread_mutex_lock(&in_mu);
	at_eof = 0;
	pthread_mutex_unlock(&in_mu);
	return lua_main(1, argv);
}

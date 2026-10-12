#ifndef SIM_H
#define SIM_H

#include <stddef.h>

/* The alyn MCU C code as a library for the Go TUI. Replaces what the Qt simulator did in
 * its own process: tick thread, game thread, merged frame, station LEDs, key input, and the
 * Lua console through the USE_GUI_CONSOLE hooks (readline / write_line), see console.c. */

#define SIM_W 64
#define SIM_H_ 32
#define SIM_STATIONS 8
#define SIM_FRAME_LEN (SIM_W * SIM_H_ * 3 + SIM_STATIONS * 4)

enum { ST_CONNECTED = 1, ST_BLINK = 2, ST_BLINK_ON = 4 };

void sim_init(void);   /* led matrix, stations connected, tick thread, console pipes */

/* games: embedded luac scripts (Qt MainWindow::onStartStopClicked) */
int         sim_game_count(void);
const char *sim_game_name(int idx);
int         sim_game_start(int idx);
int         sim_game_start_source(const char *src, size_t len);
void        sim_game_stop(void);
int         sim_game_running(void);

/* input (Qt arrawkey / mainwindow / ledcircle) */
void sim_key(int id, int is_long);
void sim_button(int station);
void sim_connect(int station, int on);

/* out: SIM_W*SIM_H_ RGB (merged channel 3), then SIM_STATIONS * {r,g,b,flags} */
void sim_frame(unsigned char *out);

/* Lua console. The REPL (lua_main) runs on the calling thread until EOF.
 * Output is a stream of chunks on console_out_fd(): kind u8 | len u32 LE | bytes. */
enum { CON_OUT = 'O', CON_PROMPT = 'P' };
int  console_out_fd(void);
int  console_run(void);
void console_send_line(const char *line);
void console_eof(void);       /* makes readline return NULL, ending console_run */
void console_write(const char *s, int len);

#endif

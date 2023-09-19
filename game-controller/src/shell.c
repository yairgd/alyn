/*
 * =====================================================================================
 *
 *       Filename:  shell.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/05/2023 06:25:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_uart.h>

#include <version.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>
#include <ctype.h>
                                                                                    
#ifdef CONFIG_ARCH_POSIX
#include <unistd.h>
#else
#include <zephyr/posix/unistd.h>
#endif

#include "system_model.h"
#include "hwctl.h"
#include "game.h"
#include "parser_task.h"


LOG_MODULE_REGISTER(app);


void timer_expired_handler(struct k_timer *timer)
{
	LOG_INF("Timer expired.");

	/* Call another module to present logging from multiple sources. */
	//foo();
}

K_TIMER_DEFINE(log_timer, timer_expired_handler, NULL);



#define CHAR_1 0x18
#define CHAR_2 0x11


static int set_bypass(const struct shell *sh, shell_bypass_cb_t bypass)
{
	static bool in_use;

	if (bypass && in_use) {
		shell_error(sh, "Sample supports setting bypass on single instance.");

		return -EBUSY;
	}

	in_use = !in_use;
	if (in_use) {
		shell_print(sh, "Bypass started, press ctrl-x ctrl-q to escape");
		in_use = true;
	}

	shell_set_bypass(sh, bypass);

	return 0;
}

static void bypass_cb(const struct shell *sh, uint8_t *data, size_t len)
{
	static uint8_t tail;
	bool escape = false;

	/* Check if escape criteria is met. */
	if (tail == CHAR_1 && data[0] == CHAR_2) {
		escape = true;
	} else {
		for (int i = 0; i < (len - 1); i++) {
			if (data[i] == CHAR_1 && data[i + 1] == CHAR_2) {
				escape = true;
				break;
			}
		}
	}

	if (escape) {
		shell_print(sh, "Exit bypass");
		set_bypass(sh, NULL);
		tail = 0;
		return;
	}
	proccess_char(sh, data, len);

//	_Z13proccess_charPK5shellPhm(sh, data, len);
#if 0
	/* Store last byte for escape sequence detection */
	tail = data[len - 1];

	/* Do the data processing. */
	for (int i = 0; i < len; i++) {
		shell_fprintf(sh, SHELL_INFO, "%02x ", data[i]);
	}
	shell_fprintf(sh, SHELL_INFO, "| ");

	for (int i = 0; i < len; i++) {
		shell_fprintf(sh, SHELL_INFO, "%c", data[i]);
	}
	shell_fprintf(sh, SHELL_INFO, "\n");
#endif
}






static int cmd_bypass(const struct shell *sh, size_t argc, char **argv)
{
	return set_bypass(sh, bypass_cb);
}

static int cmd_version(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "Zephyr version %s", KERNEL_VERSION_STRING);

	return 0;
}

//#include "getopt.h"
int cmd_led_en(const struct shell *sh, size_t argc,
		char **argv)
{

	int id = argv[1] ? atoi(argv[1]) : 0 ;

	hwctl_stop_free_run();	
	if (0 < id && id <= 8)
		hwctl_enable_node(id - 1) ;
	else {
		hwctl_disable_all_nodes();
	}
	return 0;
}

int cmd_led_blink(const struct shell *sh, size_t argc,
		char **argv)
{
	int id = argv[1] ? 1 <= atoi(argv[1]) : 0 ;
	int freq = argv[2] ? atoi(argv[2]) : 5 ;
	int timeout = argv[3] ? atoi(argv[3]) : 10 ;

	if (0 < id && id <= 8) {	
		set_free_run_delay(66);
		start_blink(id - 1,freq,timeout);
		hwctl_start_free_run();
	} else {
		hwctl_disable_all_nodes();		
	}

	return 0;
}

int cmd_led_run(const struct shell *sh, size_t argc,
		char **argv)
{
	int delay = argv[1] ? atoi(argv[1]) : 1000 ;

	set_free_run_delay(delay);
	hwctl_start_free_run();

	return 0;
}


int cmd_led_stop(const struct shell *sh, size_t argc,
		char **argv)
{
	hwctl_stop_free_run();	
	return 0;
}

int cmd_led_rgb(const struct shell *sh, size_t argc,
		char **argv)
{
	int id = argv[1] ? atoi(argv[1]) : 0 ;
	if (0 < id && id <= 8) {
		int r = argv[2] ? atoi(argv[2]) : 0 ;
		int g = argv[3] ? atoi(argv[3]) : 0 ;
		int b = argv[4] ? atoi(argv[4]) : 0 ;
		set_rgb( id - 1,  r,  g,  b);	
		hwctl_enable_node(id - 1) ;		

	}
	return 0;
}



#ifdef CONFIG_SHELL_GETOPT
static int cmd_game_start_ts(const struct shell *sh, size_t argc,
		char **argv)
{
	struct getopt_state *state;
	//char *cvalue = NULL;
	int level = 0;
	int time = 0;
	int game_id=0;
	int c;

	while ((c = getopt(argc, argv, "l:t:n:")) != -1) {
		state = getopt_state_get();
		switch (c) {
			case 'l':
				level = atoi(state->optarg);;
				break;
			case 't':
				time = atoi(state->optarg);;
				break;
			case 'n':
				game_id = atoi(state->optarg);
				break;
			case 'h':
				/* When getopt is active shell is not parsing
				 * command handler to print help message. It must
				 * be done explicitly.
				 */
				shell_help(sh);
				return SHELL_CMD_HELP_PRINTED;
			case '?':
				if (state->optopt == 't') {
					shell_print(sh,
							"Option -%c requires game index.",
							state->optopt);
				} else if (isprint(state->optopt) != 0) {
					shell_print(sh,
							"Unknown option `-%c'.",
							state->optopt);
				} else {
					shell_print(sh,
							"Unknown option character `\\x%x'.",
							state->optopt);
				}
				return 1;
			default:
				break;
		}
	}

	shell_print(sh, "game_id = %d, level = %d, time =%d",game_id, level, time);

	return 0;
}


static int cmd_led_on_ts(const struct shell *sh, size_t argc,
		char **argv)
{
	struct getopt_state *state;
	int n = 0;
	int r = 127;
	int g = 127;
	int b = 127;
	int c;

	while ((c = getopt(argc, argv, ":n:r:g:b:")) != -1) {
		state = getopt_state_get();
		switch (c) {
			case 'n':
				n = atoi(state->optarg);;
				break;
			case 'r':
				r = atoi(state->optarg);;
				break;
			case 'g':
				g = atoi(state->optarg);;
				break;
			case 'b':
				b = atoi(state->optarg);;
				break;

			case 'h':
				/* When getopt is active shell is not parsing
				 * command handler to print help message. It must
				 * be done explicitly.
				 */
				shell_help(sh);
				return SHELL_CMD_HELP_PRINTED;
			case '?':
				if (state->optopt == 't') {
					shell_print(sh,
							"Option -%c requires game index.",
							state->optopt);
				} else if (isprint(state->optopt) != 0) {
					shell_print(sh,
							"Unknown option `-%c'.",
							state->optopt);
				} else {
					shell_print(sh,
							"Unknown option character `\\x%x'.",
							state->optopt);
				}
				return 1;
			default:
				break;
		}
	}

	set_rgb (n, r, b, g);
	shell_print(sh, "n = %d, r = %d, g =%d , b = %d",n,r,g,b);

	return 0;
}
#endif

//#include "lua.h"
//#include "lauxlib.h"
//#include "lualib.h"
//extern lua_State *L;
static int cmd_game_lua(const struct shell *sh, size_t argc,
		char **argv)
{
	//lua_State *L = luaL_newstate();
	//luaL_openlibs(L);
	//printf("%s\n", argv[1]);
	//ARG_UNUSED(luaL_dostring(L, argv[1]));
		//ARG_UNUSED(luaL_dostring(L, "g = 123;print (g);"));
	//if (argc >1 && argv[1] && luaL_dostring(L, argv[1])) {
	//	fprintf(stderr, "%s\n", lua_tostring(L,-1));

	//}

	return 0;
}

static int cmd_game_list(const struct shell *sh, size_t argc,
		char **argv)
{	
	game_show_list();
	return 0;
}


static int cmd_game_start(const struct shell *sh, size_t argc,
		char **argv)
{
	int id = argv[1] ? 1 <= atoi(argv[1]) : 0 ;	

	struct game  * g = game_get_by_index(id);
	set_active_game(id);
	game_start(g);
	return 0;
}

static int cmd_game_stop(const struct shell *sh, size_t argc,
		char **argv)
{
	set_active_game(255);
	game_stop();	
	return 0;
}




SHELL_STATIC_SUBCMD_SET_CREATE(sub_game,
		/* game commands */
		SHELL_CMD(lua, NULL, "",  cmd_game_lua) ,		
		SHELL_CMD(list, NULL,"show the list of games on the device",  cmd_game_list) ,		
		SHELL_CMD(run, NULL,"starts game. Usage <game_id> ",  cmd_game_start) ,		
		SHELL_CMD(stop, NULL,"stop runnuing game.",  cmd_game_stop ),		
#ifdef CONFIG_SHELL_GETOPT
		SHELL_CMD(start, NULL, "starts game. Usage -n<game_id> t<time> -l<level>  ",  cmd_game_start) ,
		SHELL_CMD(stop, NULL, "stops game", /* cmd_game_start)*/0) ,
#endif
		SHELL_SUBCMD_SET_END
		);
		SHELL_CMD_REGISTER(game, &sub_game, "Game commands", NULL);

		/* Led Commands */
		SHELL_STATIC_SUBCMD_SET_CREATE(sub_led,
#ifdef CONFIG_SHELL_GETOPT 
		SHELL_CMD(on, NULL, "turn on led. Usage -n<game_id> t<time> -l<level>  ",  cmd_led_on_ts) ,
		SHELL_CMD(off, NULL, "turn off led", /* cmd_game_start)*/0) ,
#endif
		SHELL_CMD(en, NULL, "enable led. Usage <led id(1-8)> ",  cmd_led_en) ,
		SHELL_CMD(run, NULL, "run the led sequenct. Usage <speed(s)> ",  cmd_led_run) ,
		SHELL_CMD(blink, NULL, "start led blinking. Usage <id> <freq> <timeout> ",  cmd_led_blink) ,		
		SHELL_CMD(stop, NULL, "stop the led sequence ",  cmd_led_stop) ,
		SHELL_CMD(rgb, NULL, "chage station rgb leds. Usage <id> <r> <g> <b> ",  cmd_led_rgb) ,
		SHELL_SUBCMD_SET_END
		);
		SHELL_CMD_REGISTER(led, &sub_led, "Leds commands", NULL);

		/* version command */
		SHELL_CMD_ARG_REGISTER(version, NULL, "Show kernel version", cmd_version, 1, 0);

		/* bypass command */
		SHELL_CMD_ARG_REGISTER(bypass, NULL, "Show kernel version", cmd_bypass, 1, 0);

		


int init_shell(void)
{


#if DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_shell_uart), zephyr_cdc_acm_uart)
	const struct device *dev;
	uint32_t dtr = 0;

	dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	if (!device_is_ready(dev) || usb_enable(NULL)) {
		return 0;
	}

	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
		k_sleep(K_MSEC(100));
	}
#endif
	const struct shell * sh = shell_backend_uart_get_ptr();
	shell_use_colors_set (sh, 1);
	return 0;
}



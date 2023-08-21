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


LOG_MODULE_REGISTER(app);


void timer_expired_handler(struct k_timer *timer)
{
	LOG_INF("Timer expired.");

	/* Call another module to present logging from multiple sources. */
	//foo();
}

K_TIMER_DEFINE(log_timer, timer_expired_handler, NULL);






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

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
static int cmd_game_lua(const struct shell *sh, size_t argc,
		char **argv)
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	printf("%s\n", argv[1]);
	ARG_UNUSED(luaL_dostring(L, argv[1]));
	return 0;
}






SHELL_STATIC_SUBCMD_SET_CREATE(sub_game,
		SHELL_CMD(lua, NULL, "",  cmd_game_lua) ,		
#ifdef CONFIG_SHELL_GETOPT
		SHELL_CMD(start, NULL, "starts game. Usage -n<game_id> t<time> -l<level>  ",  cmd_game_start_ts) ,
		SHELL_CMD(stop, NULL, "stops game", /* cmd_game_start)*/0) ,
#endif
		SHELL_SUBCMD_SET_END
		);
		SHELL_CMD_REGISTER(game, &sub_game, "Game commands", NULL);

		SHELL_STATIC_SUBCMD_SET_CREATE(sub_led,
#ifdef CONFIG_SHELL_GETOPT 
				SHELL_CMD(on, NULL, "turn on led. Usage -n<game_id> t<time> -l<level>  ",  cmd_led_on_ts) ,
				SHELL_CMD(off, NULL, "turn off led", /* cmd_game_start)*/0) ,
#endif
				SHELL_CMD(en, NULL, "turn on led. Usage -n<game_id> t<time> -l<level>  ",  cmd_led_en) ,
				SHELL_CMD(run, NULL, "turn on led. Usage -n<game_id> t<time> -l<level>  ",  cmd_led_run) ,
				SHELL_CMD(stop, NULL, "turn on led. Usage -n<game_id> t<time> -l<level>  ",  cmd_led_stop) ,
				SHELL_CMD(rgb, NULL, "chage station rgb leds. Usage <id> <r> <g> <b> ",  cmd_led_rgb) ,




				SHELL_SUBCMD_SET_END
				);
				SHELL_CMD_REGISTER(led, &sub_led, "Leds commands", NULL);
				SHELL_CMD_ARG_REGISTER(version, NULL, "Show kernel version", cmd_version, 1, 0);


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
	return 0;
}


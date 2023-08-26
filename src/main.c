/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include <zephyr/kernel.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

#include "shell.h"


#include <zephyr/fs/fs.h>
#include <zephyr/drivers/gpio/gpio_emul.h>

#include <stdlib.h>

#include "lauxlib.h"
#include "lualib.h"
#include "hwctl.h"
#include "system_model.h"
#include "game.h"

int _open() {
	return 0;
}

	volatile const struct gpio_dt_spec spec = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(enables),
                                                     gpios, 0);

const char *lua1 = " \
for i = 10,1,-1 \
do \
   print(i, 'str ' .. tostring(i)) \
   --[[ assert((i %  2) == 0)  --]] \
end \
";

const char *lua2 = " \
function wait(seconds)\
    local start = os.time() \
    while os.time() - start < seconds do \
    end \
end \
for i = 80,1,-1 \
do \
   game.enable(i % 8 + 1)\
   game.delay(1000000); \
end \
";



// #include <zephyr/posix/time.h>
  //#include <zephyr/posix/sys/time.h>


//https://blog.devgenius.io/how-to-add-your-c-library-in-lua-46fd246f0fa8
//lua_State *L;
int main(void)
{
//	set_free_run_delay(100);
//		k_sleep(K_MSEC(500));		
//
	//start_blink(1,2,10);
	//while (1) {
	//int ret = clock_gettime(CLOCK_REALTIME, &rts);

	//	k_sleep(K_MSEC(500));		
//	}
	//fwrite("test", sizeof(char), sizeof("test"), stderr);
	game_init();
	init_shell();
	while (1) {
		k_sleep(K_MSEC(500));		
	}
	//game_show_list() ;
	// L = luaL_newstate();
  	//luaL_openlibs(L);
  	//int n4 = luaL_dostring(L, "wait(5);");
	//(void)n4;
	//int n7 = luaL_dostring(L, "debug.traceback()");
	//(void)n7;
	//fprintf(stderr, "%s\n", lua_tostring(L,-1));
  	//ARG_UNUSED(luaL_dostring(L, "print('aaaaa');game = require('game');print(game);game.enable(2);print('Hello from Lua');"));
  //	int n2 = luaL_dostring(L, "a=1;b=2;print('assert', assert(a==b));g=require('game')");
//	(void)n2;

//	if (luaL_dostring(L, lua)) {
//		fprintf(stderr, "%s\n", lua_tostring(L,1));
//	}

	//printk("Hello World! %s\n", CONFIG_BOARD);
	
#if 1
//struct timespec rts, nts;


	while (1) {
	//int ret = clock_gettime(CLOCK_REALTIME, &rts);

		k_sleep(K_MSEC(500));		
	//	ret = clock_gettime(CLOCK_REALTIME, &nts);

	//	 int64_t delta =
	//		((int64_t)rts.tv_sec * NSEC_PER_SEC -
	//		 (int64_t)nts.tv_sec * NSEC_PER_SEC) +
	//		((int64_t)rts.tv_nsec - (int64_t)nts.tv_nsec);
	//	k_sleep(K_MSEC(1000));		


	}
#endif
	return 0;
}

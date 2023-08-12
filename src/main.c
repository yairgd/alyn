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

/* the gpios of node enable */
struct gpio_dt_spec en_station[] = {
        DT_FOREACH_PROP_ELEM_SEP(DT_NODELABEL(enables), gpios,
                                 GPIO_DT_SPEC_GET_BY_IDX, (,))
};


#include <zephyr/fs/fs.h>

#include <stdlib.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


int _open() {
	return 0;
}

const char *lua = " \
for i = 10,1,-1 \
do \
   print(i, 'str ' .. tostring(i)) \
   --[[ assert((i %  2) == 0)  --]] \
end \
";
int main(void)
{

		
	lua_State *L = luaL_newstate();
  	luaL_openlibs(L);
  	int n1 = luaL_dostring(L, "print('Hello from Lua')");
	(void)n1;
  	int n2 = luaL_dostring(L, "a=1;b=1;print('assert', assert(a==b))");
	(void)n2;
  	int n3 = luaL_dostring(L, lua);
	(void)n3;




	//printk("Hello with Lua!\n");

	lua_close(L);
	
	// configure the LED pin as output
	for (int i =0;i < 2; i++)
	  	gpio_pin_configure_dt(&en_station[i], GPIO_OUTPUT);


	printk("Hello World! %s\n", CONFIG_BOARD);
	init_shell();
	return 0;
}

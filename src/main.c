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

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "hwctl.h"
#include "system_model.h"

int _open() {
	return 0;
}

	volatile const struct gpio_dt_spec spec = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(enables),
                                                     gpios, 0);

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
  	ARG_UNUSED(luaL_dostring(L, "print('Hello from Lua')"));
  	int n2 = luaL_dostring(L, "a=1;b=1;print('assert', assert(a==b))");
	(void)n2;
  	int n3 = luaL_dostring(L, lua);
	(void)n3;



	//printk("Hello with Lua!\n");

	lua_close(L);

	



//const struct gpio_dt_spec spec1 = GPIO_DT_SPEC_GET_BY_IDX(DT_NODELABEL(led0),
  //                                                   gpios, 0);

	//gpio_emul_input_set(spec.port, 0, GPIO_OUTPUT);
	//gpio_pin_configure(spec.port, 0, GPIO_OUTPUT);


	//pio_pin_configure_dt(&en_station[0], GPIO_OUTPUT);

#if 0
	// configure the LED pin as output
	for (int i =0;i < 1; i++)
	  	gpio_pin_configure_dt(&en_station[i], GPIO_OUTPUT);
#endif
	printk("Hello World! %s\n", CONFIG_BOARD);
	init_shell();

	while (1) {
		k_sleep(K_MSEC(1000));		
	}

	return 0;
}

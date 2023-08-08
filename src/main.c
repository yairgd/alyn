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


int main(void)
{

		
	// configure the LED pin as output
	for (int i =0;i < 2; i++)
	  	gpio_pin_configure_dt(&specs[i], GPIO_OUTPUT);


	printk("Hello World! %s\n", CONFIG_BOARD);
	init_shell();
	return 0;
}

/*
 * =====================================================================================
 *
 *       Filename:  gpio.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/19/2024 08:57:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>


#include "system_model.h"
#include <stdio.h>

#define GPIO_NODE DT_NODELABEL(gpioa)

#define BUTTON_UP_NODE DT_NODELABEL(user_button_up)
#define BUTTON_DOWN_NODE DT_NODELABEL(user_button_down)
#define BUTTON_LEFT_NODE DT_NODELABEL(user_button_left)
#define BUTTON_RIGHT_NODE DT_NODELABEL(user_button_right)
#define BUTTON_MIDDLE_NODE DT_NODELABEL(user_button_middle)
//#define BUTTON_STB_NODE DT_NODELABEL(station_button)

#define GET_GPIO_PIN(node) DT_GPIO_PIN(node, gpios)
#define GET_GPIO_FLAGS(node) (GPIO_INPUT | DT_GPIO_FLAGS(node, gpios)  | GPIO_INT_EDGE_BOTH)

/* size of stack area used by each thread */
#define STACKSIZE 4096

/* scheduling priority used by each thread */
#define PRIORITY 6




struct button {
	struct gpio_callback button_cb_data;
	const char *label;
	const struct gpio_dt_spec spec;
	int64_t press_time;
	int id;
	int state;
};

#define BUTTON_SPEC(node,id_) { .label = DT_LABEL(node), .spec = GPIO_DT_SPEC_GET_OR(node, gpios, {0}), .press_time = 0,.id=(id_) }

static struct button buttons[] = {
	BUTTON_SPEC(BUTTON_MIDDLE_NODE,0),	
	BUTTON_SPEC(BUTTON_UP_NODE,    1),		
	BUTTON_SPEC(BUTTON_LEFT_NODE,  2),
	BUTTON_SPEC(BUTTON_RIGHT_NODE, 3),	
	BUTTON_SPEC(BUTTON_DOWN_NODE,  4),
//	BUTTON_SPEC(BUTTON_STB_NODE,   5),
};


void buttons_init(void)
{
	int ret;

	for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
		ret = gpio_pin_configure_dt(&buttons[i].spec, GPIO_INPUT);
		if (ret != 0) {
			printk("Error %d: failed to configure pin %d for %s\n", ret, buttons[i].spec.pin, buttons[i].label);
			return;
		}

		if (!device_is_ready(buttons[i].spec.port)) {
			printk("Error: Button %s GPIO port %s is not ready\n", buttons[i].label, buttons[i].spec.port->name);
			return;
		}

	}

}


void buttons_thread (void *p1,void *p2, void *p3)
{
	int val;
	int64_t duration;
	buttons_init();	
	int64_t now = 0;
	while (1) {
		//int64_t now = k_uptime_get();				 		
		for (int i = 0; i < ARRAY_SIZE(buttons); i++) {
			val = gpio_pin_get_dt(&buttons[i].spec);	
			if (val && buttons[i].state==0) {
				 buttons[i].state=1;
				 buttons[i].press_time = now;
			} else if (buttons[i].state>0 && buttons[i].state<3  && val) {
				duration = now - buttons[i].press_time;
				if (duration >= 1000 && buttons[i].state == 2) {
					buttons[i].state = 3;
					set_long_key(buttons[i].id, 1);	
				} else if (duration >= 10 && buttons[i].state == 1) {
					buttons[i].state = 2;
					set_key(buttons[i].id, 1);					
				}
			} else if (val==0 && buttons[i].state) {
				if (buttons[i].state == 3) {
					printf("Button %s (GPIO %d) long press (%lld ms)\n", buttons[i].label, buttons[i].spec.pin, duration);
				} else if  (buttons[i].state == 2) {
					printf("Button %s (GPIO %d) short press (%lld ms)\n", buttons[i].label, buttons[i].spec.pin, duration);	
									
				}
				buttons[i].state = 0;
				

			}
			
		}
		now++;
		k_sleep(K_MSEC(1)); // Wait for 1 second
	}
}



K_THREAD_DEFINE(buttons_thread_id, STACKSIZE, buttons_thread, NULL , NULL, NULL, PRIORITY, 0, 0);

